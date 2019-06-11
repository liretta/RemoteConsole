# About Remote Console

  It is application for executing command line's commands remote. 
  
  ## How it works
  
  Client run the application, connect to the server and excecute command remote. All commands, that client entered in his window will be excecute in remote PC
  
  Encrypted communication channel is used.
  
  # Installation instruction 
  
  1. To run Remote Console will be enough to download or clone folder **ReleaseFiles**.
  2. Extract all archive content into any directory.
  3. Go to the folder **Excecutable**
  4. Run **RemoteConsoleUI.exe**
  
  **Done!**
  <hr>
  
  
# Interface

### Mode selection window
![chooseMod](https://user-images.githubusercontent.com/38506033/59277001-09c67800-8c68-11e9-8ba1-9303a215de44.JPG)

**Server** will start the programm as server in your PC IP-address. If there is some problem with command "ipconfig" in your PC, the server will be run in the localhost 127.0.0.1

**Client**: for start as client you need enter server's IP-adress.

## Server window
Server's window has two parts: _Commands_ and _Errors_

Inside _Commands_ will be all client's commands and information regarding current server status.

![server](https://user-images.githubusercontent.com/38506033/59277210-75104a00-8c68-11e9-9ece-e8a75d726fec.JPG)

While client isn't connected or if client is disconnected, server will be in status *Waiting for connect*

**Attention!** The application supports only one client connection in the same time!

## Client windows
### Connetion window
![ip](https://user-images.githubusercontent.com/38506033/59278150-26fc4600-8c6a-11e9-8d22-e17c4576cff3.JPG)

For connection client should enter server's IP-adress. If adress isn't correct or server is down, client will get error window:

![cannot](https://user-images.githubusercontent.com/38506033/59278348-7cd0ee00-8c6a-11e9-8864-4d92140d3a0f.JPG)

If connection is successful, will open authorization window.

### Authorization window
![authorization](https://user-images.githubusercontent.com/38506033/59278528-d0dbd280-8c6a-11e9-958c-ebc7ffb7d7e6.JPG)

**Attention!** passwords **hash** are keeped into file *user.ini*

If you need test the application, use test login **user** and password **user** or login **nata** & password **nata**.

For create new authorisation file, please, contact us liretta26@gmail.com - we will send you additional application.

### Main client window
Main client window has three parts: *Command*, *Output* and *Errors*

![Client](https://user-images.githubusercontent.com/38506033/59279349-717ec200-8c6c-11e9-88de-641b46b08d06.JPG)

For using enter some command into the field *Command* and press the botton *Excecute* via mouse.

<hr>
<hr>

# Architecture and modules

### Networker module
Network are implemented in interface class **INetworker**, base common class **BaseNetworker** are inherited from interface;
and classes **ClientNetworker** and **ServerNetworker** are inherited from **BaseNetworker**. All networker classes use winsock API 

- **BaseNetworker**: base abstract class for networking. Methods: `send(const std::vector<char> &a_message)`, `receive(std::vector<char> &a_message)`, `shutdownSend()`, `shutdownReceive()`, `shutdownSendReceive()` and protected method `init_library()`.
- **ClientNetworker**: create connection in the client side. Methods: `init(const std::string &def_adr = "127.0.0.1")` call private methods `create_socket(const std::string &def_adr)` and `create_connection()`, `init_library()` from base_networker;
- **ServerNetworker**: create connecting with client. Methods: `init(const std::string &def_adr = "127.0.0.1")` call private methods `create_socket(const std::string &def_adr)`, `create_connection()` `get_my_ip(std::string &ip_addr)`.
  has a friend class **Server**;

### Authorization module
Authorization module are implemented in interface class **ILogger**, and inharitance classes **ServerLogger** & **ClientLogger**

- **ILogger**: abstract interface class for authorization functionality. Methods: `check_password(auth_data const &log_pair)` and `check_password(const std::string &log, const std::string &pass)`;
- **ServerLogger**: class for checking authorization data from external file *user.ini*. Methods: `check_password(auth_data const &log_pair)`, `check_password(const std::string &log, const std::string &pass)`, and private method `load_auth_data_from_file(std::unordered_map<std::string, unsigned long>& auth_list)`. Open file, check user authorisation data and data from file, return the result.
- **ClientLogger**: stub for server_logger. Get login+pass, send it to the server, and return result. Methods: `check_password(auth_data const &log_pair)` and `check_password(const std::string &log, const std::string &pass)`;

### RPC module
RPC module are implemented in interfase class *Marshaller* 

- **Marshaller**: pack wsting message in correct format for excecute. All message will have **Type** from enumeration: `Authorization`, `Command`, `Error`, `Result`, `WrongModeSymbol`;
Static methods: `getMode(const std::wstring& input)`, `unpackAuthorizationData(const std::wstring&)`, `packAuthorizationData(const std::wstring&, const std::wstring&)`, 
`unpackMessage(Type, const std::wstring&)`, `packMessage(Type, const std::wstring&)`, `packResult(bool)`;

### Executor module
Executor module are implemented in interface class **IServerExecutor**, and inharitence classes **ServerExecutor** and **ClientExecutor**

- **ServerExecutor**: execute command in hidden command line. Methods: `initialize()` - Set up security attributes and create a pipe, `execute(const std::wstring& command)` - Execute Unicode command in a new sub-process, `getResult()` - Get output from the sub-process and transform it to Unicode, `isInitialized()`, 
and protected methods `send_error_message()`, `create_sub_process(PROCESS_INFORMATION& process_info, wchar_t* w_command)`.  
- **ClientExecutor**: stub for server_executor. Method: `execute(const std::wstring& w_command)`.

### Encryption module
Encryption module uses the CryptoPP in interface class **ICrypto**, and inheritence classes **ServerCryptor** and **ClientCryptor**.

- **ServerCryptor**: functionality for encrypting and decrypting messages in the server side. Generate and validate public&private keys, set synchronous key
can encrypt/decrypt message with sychronous key, decrypt message via private key. 
Methods:  `generateKey()` for asynchronous key, `getPublicKey()` return public key, `setKey(const std::vector<char>& key, const std::vector<char>& iv)` set synchronous key and his initialized vector, `encrypt(const std::wstring& message)` and `decrypt(const std::vector<char>& message)` used synchronous encription. Private methods `key_decrypt(const std::vector<char>& key)` and `iv_decrypt(const std::vector<char>& iv)` encrypt and decrypt synchronous key via public key.
- **ClientCryptor**: encryption functionality in client side. Get public key and encrypt message with it, generate synchronous key and initialize vector for it, encryption and decryption message via synchronous key, pack public key for sending. For repack using `repackPublicKey(const std::vector <char>& v_key)`.
Methods: `generateKey()` -generate synchronous key, `setKey(const std::vector<char>& v_key)` - set public key from server, `keyEncrypt()`and `ivEncrypt()` encription synchronous key via public key, `encrypt(const std::wstring& message)` and `decrypt(const std::vector<char>& message)` for encription message via synchronous key. 
 
### General classes for GUI
All functionality are inside the RemoteConsole static library, and are implemented in two general classes **Server** and **Client**, wich are inharited from **IServer* and **IClient**.

- **Server**: united functionality server-side. Methods: `waitingForConnection()` - waiting in "accept" socket (using private method `create_connection` from **networker**, `logIn()` - get log/pass, check it, return answer to client, `sendKey()` - generate and send public key to the client, `getKey()` - get synchronous key from client and initialize vector, `run()` - running data exchange between client and server while connection is present.
- **Client**: united functionality client-side. Get opportunity to logIn, execute command and exchange keys. 
Methods: `getExecutor()`, `getLogger()`, `getNetworker()`, `setCryptoKey()` - receive key from server, validate and save it, `sendCryptoKey()` - generate synchronous key and send it to the server
