#include "statesmanager.hpp"
#include <chrono>
#include <thread>

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

StatesManager::StatesManager()
{
	sf::Image cursorImage, scaledCursor;
	cursorImage.loadFromFile("textures/cursor.png");
	scaledCursor.create(cursorImage.getSize().x * CON::PIXEL_SIZE, cursorImage.getSize().y * CON::PIXEL_SIZE);
	for (unsigned int x = 0; x < cursorImage.getSize().x; x++) {
		for (unsigned int y = 0; y < cursorImage.getSize().y; y++) {
			for (int a = 0; a < CON::PIXEL_SIZE; a++) {
				for (int b = 0; b < CON::PIXEL_SIZE; b++) {
					scaledCursor.setPixel(x * CON::PIXEL_SIZE + a, y * CON::PIXEL_SIZE + b, cursorImage.getPixel(x, y));
				}
			}
		}
	}
	cursor.loadFromPixels(scaledCursor.getPixelsPtr(), scaledCursor.getSize(), sf::Vector2u(0, 0));

	std::thread tcpThread(&StatesManager::handleTcp, this);
	tcpThread.detach();
}

StatesManager::~StatesManager()
{
	if (state != NULL)
		delete state;

	state = NULL;
}

int StatesManager::run()
{
	auto id = getHardwareId();

	sf::ContextSettings openGLSettings;
	openGLSettings.antialiasingLevel = 8;
	auto width = sf::VideoMode::getDesktopMode().width;
	sf::RenderWindow window(sf::VideoMode(int(width * 2 / 3.f), int(width * 3 / 8.f)), CON::GAME_NAME, sf::Style::Default, openGLSettings);
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursor(cursor);

	state = new HomeState(window, gameInfo, server);
	auto lastTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	while (window.isOpen()) {
		sf::Event e;
		std::vector<sf::Event> events;
		while (window.pollEvent(e)) {
			//toggle fullscreen
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter && e.key.alt) {
				sf::View view = window.getView();
				if (window.getSize().x == sf::VideoMode::getFullscreenModes()[0].width)
					window.create(sf::VideoMode(int(width * 2 / 3.f), int(width * 3 / 8.f)), CON::GAME_NAME, sf::Style::Default, openGLSettings);
				else
					window.create(sf::VideoMode::getDesktopMode(), CON::GAME_NAME, sf::Style::Fullscreen, openGLSettings);
				window.setView(view);
				window.setVerticalSyncEnabled(true);
				window.setKeyRepeatEnabled(false);
				window.setMouseCursor(cursor);
			}

			events.push_back(e);
		}

		auto time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		int whatHappened = 0;
		if (!isTransitioning)
			whatHappened = state->update(events, int(std::max(time - lastTime, long long(1))) / 1000.f);
		lastTime = time;

		//draw if nothing happened
		if (whatHappened == 0) {
			state->draw();
			window.display();
		}
		//from home-state go game-state
		else if (whatHappened == 1) {
			delete state;
			state = new GameState(window, gameInfo, settings, server, weaponManager);
		}
		//from game-state (pause-state) to home-state
		else if (whatHappened == 2) {
			sf::Packet p;
			p << SEND::EXITED_LOBBY;
			server.tcp.send(p);

			delete state;
			state = new HomeState(window, gameInfo, server);
		}
		//from home-state to shop
		else if (whatHappened == 3) {
			delete state;
			state = new ShopState(window, weaponManager);
		}
	}

	return 0;
}

void StatesManager::handleTcp()
{
	sf::Packet p;
	sf::Uint8 res = 0;
	while (res != REC::CONNECTED && res != REC::VERSION_INCOMPATIBLE) {
		while (server.tcp.connect(CON::SERVER_IP, CON::TCP_SERVER_PORT, sf::seconds(10)) != sf::Socket::Done)
			sf::sleep(sf::seconds(1));

		p.clear();
		p << SEND::CONNECT << std::string("dev5");
		server.tcp.send(p);

		p.clear();
		if (server.tcp.receive(p) == sf::Socket::Done)
			p >> res;
	}

	if (res == REC::CONNECTED)
		server.isConnected = true;
	else if (res == REC::VERSION_INCOMPATIBLE) {
		server.versionCompatible = false;
		return;
	}

	while (true) {
		sf::Packet p;
		sf::Socket::Status status = server.tcp.receive(p);
		if (status == sf::Socket::Done)
			server.packets.push(p);
		else if (status == sf::Socket::Disconnected)
			server.isConnected = false;
	}
}

std::array<unsigned char, 20> StatesManager::getHardwareId()
{
	std::array<unsigned char, 20> id = { 0 };

	DWORD volumeSerialNumber = 0;
	BOOL result = GetVolumeInformationA("C:\\", 0, 0, &volumeSerialNumber, 0, 0, 0, 0);
	if (result) {
		char serialNumber[9];
		sprintf_s(serialNumber, "%08X", volumeSerialNumber);
		
		id[0] = unsigned char(volumeSerialNumber / (256 * 256 * 256));
		id[1] = unsigned char(volumeSerialNumber / (256 * 256) % 256);
		id[2] = unsigned char(volumeSerialNumber / (256) % (256 * 256));
		id[3] = unsigned char(volumeSerialNumber % (256 * 256 * 256));
	}
	else {
		std::cout << "failed to retrieve volume serial number, error: " << GetLastError();
		std::exit(101);
	}

	HRESULT hres;

	//initialize COM
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		std::cout << "Failed to initialize COM library. Error code = 0x" << std::hex << hres;
		std::exit(101);
	}

	//set general COM security levels
	hres = CoInitializeSecurity(0, -1, 0, 0, RPC_C_AUTHN_LEVEL_DEFAULT, 
		RPC_C_IMP_LEVEL_IMPERSONATE, 0, EOAC_NONE, 0);
	if (FAILED(hres)) {
		std::cout << "Failed to initialize security. Error code = 0x" << std::hex << hres;
		CoUninitialize();
		std::exit(101);
	}

	//obtain the initial locator to WMI
	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hres)) {
		std::cout << "Failed to create IWbemLocator object. Error code = 0x" << std::hex << hres;
		CoUninitialize();
		std::exit(101);
	}

	//connect to WMI through the IWbemLocator::ConnectServer method
	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), 0, 0, 0, 0, 0, 0, &pSvc);
	if (FAILED(hres)) {
		std::cout << "Could not connect. Error code = 0x" << std::hex << hres;
		pLoc->Release();
		CoUninitialize();
		std::exit(101);
	}

	//set security levels on the proxy
	hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, 0, 
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, 0, EOAC_NONE);
	if (FAILED(hres)) {
		std::cout << "Could not set proxy blanket. Error code = 0x" << std::hex << hres;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		std::exit(101);
	}

	//use the IWbemServices pointer to make requests of WMI
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT UUID FROM Win32_ComputerSystemProduct"), 
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 0, &pEnumerator);
	if (FAILED(hres)) {
		std::cout << "Query for UUID failed. Error code = 0x" << std::hex << hres;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		std::exit(101);
	}

	//get the data from the query
	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;
	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (uReturn == 0)
			break;

		//get the value of the UUID property
		VARIANT vtProp; 
		VariantInit(&vtProp);
		hr = pclsObj->Get(L"UUID", 0, &vtProp, 0, 0);
		std::wstring ws(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
		std::string uuid(ws.begin(), ws.end());

		VariantClear(&vtProp);
		pclsObj->Release();
		pSvc->Release();
		pLoc->Release();
		pEnumerator->Release();
		CoUninitialize();

		while (uuid.find('-') != std::string::npos)
			uuid.erase(uuid.begin() + uuid.find('-'));

		auto toInt = [](unsigned char c) {
			if (c >= 48 && c <= 57)
				return c - 48;
			else if (c >= 65 && c <= 70)
				return c - 55;
			else if (c >= 97 && c <= 102)
				return c - 87;
		};
		for (size_t i = 0; i < 16; i++)
			id[4 + i] = toInt(uuid[i * 2]) * 16 + toInt(uuid[i * 2 + 1]);

		return id;
	}

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	std::cout << "UUID not found";
	std::exit(101);
	return id;
}
