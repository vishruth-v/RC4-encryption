# RC4 Encryption Project (SecureComm)

C++ project to create a simple command line based messenger service

## Elements of the project

- Accounts
- Messages
- Encryption using RC4 algorithm

### Accounts

The service allows creating and deleting of accounts which will be saved in a binary file called `users.dat` with the usernames and password which can then be accessed at any time

### Messges

Any user who is logged in can then send text messages to another valid user present in the `users.dat` file by attaching a `.txt` file
This `.txt` file would then be encrypted with the RC4 algorithm and sent
When the receiver logs in and wishes to view message, the `.txt` file would be decrypted and displayed

### Encryption using RC4 algorithm

The RC4 algirthm is a string cipher encryption algorithm that creates a random cipher key string using KSA. It differs from Vigenere cipher as in Vigener, the cipher key string is not random and has to be entered by user.
