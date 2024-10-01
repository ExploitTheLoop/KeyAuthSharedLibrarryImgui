
# Android C++ Shared Library with KeyAuth.cc and LicenseAuth.host Integration

## Overview

This project is a C++ shared library for Android, designed to integrate with two login systems:

- **[KeyAuth.cc](https://keyauth.cc/)** (Premium)
- **[LicenseAuth.host](https://licenseauth.host/)** (Free)

The library supports both login and chat functionality using the respective APIs from these websites.

### Features

- **User Authentication**: Supports both KeyAuth.cc (premium) and LicenseAuth.host (free) login systems.
- **Chat API Integration**: Integrated chat functionality to allow users to communicate within the app.
- **Modular Design**: Can easily be expanded with other APIs or features.
- **Cross-Platform**: Built for Android using C++ with support for integration in other environments.

## Requirements

- **Android SDK and NDK**: Ensure that you have both Android SDK and NDK installed.
- **CMake**: Required for building the shared library.
- **KeyAuth/LicenseAuth API Access**: You will need API credentials for both KeyAuth.cc and LicenseAuth.host to test or use the authentication features.
- **JNI Integration**: The Android application should interface with the shared library via JNI.

## Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/your-repository.git
   cd your-repository
   ```

2. Set up your Android environment:

   Ensure that you have the necessary Android SDK and NDK installed. Update the `local.properties` file in your Android project to point to your SDK and NDK locations if necessary.

3. Build the shared library:

   This project uses CMake to build the shared library. You can modify the `CMakeLists.txt` file to suit your project setup.

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Modify your Android app:

   Integrate the library into your Android app via JNI. Make sure to include the correct JNI functions to call the login and chat functionalities.

5. Configure API credentials:

   You will need to add your KeyAuth.cc and LicenseAuth.host API credentials into the code. These should be placed in the appropriate locations for login calls.

## Usage

### KeyAuth.cc Integration

This integration allows you to authenticate users using the premium KeyAuth.cc API. Ensure that your API credentials are configured properly in the library.

### LicenseAuth.host Integration

LicenseAuth.host provides a free alternative for authentication. To switch between KeyAuth and LicenseAuth, update the API endpoint and credentials in the library's source code.

### Chat API

The chat functionality is integrated to allow users to communicate in real-time. This API is initialized and accessed similarly to the authentication APIs.

## File Structure

```
.
├── src
│   ├── KeyAuth.hpp        # KeyAuth.cc API integration
│   ├── licenseauth.hpp      # LicenseAuth.host API integration
├── CMakeLists.txt           # Build configuration
├── README.md                # Project documentation
└── LICENSE                  # License file
```

## API Documentation

### KeyAuth Login Example

```cpp
bool login(const std::string& username, const std::string& password) {
    // Call the KeyAuth.cc login API and return the result
}
```

### LicenseAuth Login Example

```cpp
void CheckKey()
{
	if (!isrequestcompleted)
	{
		return;
	}
	showCircle = true;
	isrequestcompleted = false;
	authtextcolor = { 1.00f, 1.00f, 1.00f, 1.00f };
	authtext = ("Preparing for authentication...");
	
	if (!iskeyauthinited)
	{
		keyAuth.init();
		iskeyauthinited = true;
	}
	int returnsvr = keyAuth.license(username);
	if (returnsvr == 0)
	{
		
		authtextcolor = { 0.07f,0.52f,0.17f,1.0f };
		authtext = ("Activated successfully starting...");
		notificationSystem.AddNotification("Welcome to Osiris Hax", 4000);
        std::this_thread::sleep_for(std::chrono::seconds(1));
		showCircle = false;
	     iLogin = true;
		return;
	}
	else if (returnsvr == 1)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("This license has been banned.");
	}
	else if (returnsvr == 10)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Connection failed. Unable to connect to host.");
		isrequestcompleted = true;
		return;
	}
	else if (returnsvr == 2)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Device ID is not match.");
	}
	else if (returnsvr == 3)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Invalid key.");
	}
	else if (returnsvr == 4)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("License is expired.");
	}
	else
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Invalid key.");
	}

	isrequestcompleted = true;
	return;
}
```

### Chat API Example

```cpp
void Messages()
{
    while (true)
    {
        if (iLogin)
        {
            ChatMessages = keyAuth.chatget(XorStr("Test")); //chat name
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void GetNewMessages()
{
    std::string lastMessage;
    while (true)
    {
       if (iLogin)
        {

            if (!ChatMessages.empty())
            {
                if (lastMessage != ChatMessages.back().author)
                {
                    lastMessage = ChatMessages.back().author;
					std::string msgg = "New Message [" + ChatMessages.back().message + "] : \n" + ChatMessages.back().author;
                    notificationSystem.AddNotification(msgg.c_str(), 3000);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void SendMessages()
{
    while (true)
    {
        if (iLogin && selectedTab == 1)
        {
        
            keyAuth.chatsend(XorStr("Test").c_str(), input); //chat name
		    input.clear();
            std::this_thread::sleep_for(std::chrono::seconds(1));
			return;
			
        }
    }
}
```

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests for new features, bug fixes, or improvements.

1. Fork the repository
2. Create your feature branch: `git checkout -b feature/new-feature`
3. Commit your changes: `git commit -m 'Add new feature'`
4. Push to the branch: `git push origin feature/new-feature`
5. Submit a pull request

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [KeyAuth.cc](https://keyauth.cc) for their premium authentication services.
- [LicenseAuth.host](https://licenseauth.host) for providing free authentication solutions.
