// crpto rc4.cpp : Defines the entry point for the console application.

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <string.h>
using namespace std;

#define ARRLEN 256;


char S[256];
int I, J; //Counters throught whole program


		  //Swap two elements of array
void swap(char *a, char *b)
{
	char temp = *a;
	*b = *a;
	*a = temp;
}

void writefile(char *infile);
void switchfiles_encrypt(char* infile, char* newfile);
void encrypt(char *, char *, char *);
void displayfile(char* file);

class message
{
	char sender[30];
	char receiver[30];
	char attachment_name[40];
	char key[40];
public:
	char* getsender() { return sender; }
	char* getreceiver() { return receiver; }
	char* getattachment() { return attachment_name; }
	char* getkey() { return key; }

	void display()
	{
		cout << "From:" << sender;
		cout << "\nTo:" << receiver;
		cout << "\nAttachment File name:" << attachment_name;
	}
	void changekey(char* newkey) { strcpy_s(key, newkey); }
	message(char* from, char* to, char* attachment, char* keyb)
	{
		strcpy_s(sender, from);
		strcpy_s(receiver, to);
		strcpy_s(attachment_name, attachment);
		strcpy_s(key, keyb);
	}
	message() {}
};

class user
{
	char username[30];
	char password[16];
	char received_file_name[30];
public:
	char* getusername() { return username; }
	char* getpass() { return password; }
	char* getrecfilename() { return received_file_name; }
	void signup();
	int login();
	void save_details();
	void sendfile();
	char* generate_key_send(message ob);
	void display_received();
	void openfile();
	void display()
	{
		cout << username << password << received_file_name;
	}
}ob;


//send a message with attached file
void user::sendfile()
{
	char key[40];
	int f = 0;
	fstream fread;
	fstream f1;
	fread.open("user.dat", ios::in | ios::binary);
	user readob;
	char receiver[30];
	cout << "\nFrom:" << username;
	cout << "\nTo:"; cin.ignore(); gets(receiver); cout << endl;

	while (fread.read((char *)&readob, sizeof(user)))
	{
		if (!(strcmp(receiver, readob.getusername())))
		{
			char to[30];
			char choice;
			char filename[30];
			char encrypted_filename[30];
			cout << "Enter file name of attachment:"; cin >> filename;

			cout << "\nWant to enter data into this file?";
			cin >> choice;
			if (choice == 'y' || choice == 'Y')
				writefile(filename);

			strcpy_s(to, readob.getusername());

			strcpy_s(encrypted_filename, filename);
			strcat_s(encrypted_filename, "_encrypted.txt");
			switchfiles_encrypt(filename, encrypted_filename);

			char tempstr[30];
			strcpy_s(tempstr, "");
			message m1(username, to, filename, tempstr);
			strcpy_s(key, generate_key_send(m1));
			m1.changekey(key);
			m1.display();

			encrypt(encrypted_filename, filename, key);

			cout << m1.getattachment();
			cout << m1.getattachment();
			cout << readob.getrecfilename();
			f1.open(readob.getrecfilename(), ios::app | ios::binary);
			f1.seekg(0, ios::beg);
			f1.write((char*)&m1, sizeof(message));

			fstream f2;
			f2.open(encrypted_filename, ios::out);
			f2 << "THIS FILE DOES NOT EXIST";
			f2.close();

			cout << "\n\nEncrypted file is:";
			displayfile(filename);
		}
	}
	cout << "\n\nENCRYPTED AND SENT SUCCESFULLY";
	fread.close();
	f1.close();
}

//display all received messages
void user::display_received()
{
	strcpy_s(received_file_name, username);
	strcat_s(received_file_name, "_received.dat");
	int cnt = 0;
	message m1;
	fstream fread;
	fread.open(received_file_name, ios::in | ios::binary);
	cout << "Received file name is:" << received_file_name;
	while (fread.read((char*)&m1, sizeof(message)))
	{
		cout << "\n\nMessage no. " << cnt + 1 << endl;
		m1.display();
		cnt++;
	}
}

//asks which message to open, decrypts attachment, and prints it
void user::openfile()
{
	char decrypted_filename[30];
	char key[40];
	char attachment[70];
	message m1;
	int no;
	display_received();

	cout << "\nEnter message no. you wish to open"; cin >> no;
	fstream fread;
	fread.open(received_file_name, ios::in | ios::binary);
	fread.seekg(((no - 1) * sizeof(message)), ios::beg);
	fread.read((char*)&m1, sizeof(m1));
	cout << m1.getattachment();
	strcpy_s(attachment, m1.getattachment());
	strcpy_s(key, m1.getkey());

	fstream readattach;
	readattach.open(attachment, ios::in | ios::binary);
	strcpy_s(decrypted_filename, attachment);
	strcat_s(decrypted_filename, "_decrypted.txt");
	switchfiles_encrypt(attachment, decrypted_filename);
	encrypt(decrypted_filename, attachment, key);
	cout << "\n\nDecrypted file is:";
	displayfile(attachment);
	cout << "\n\nDECRYPTED AND OPENED SUCCESSFULY";
	fread.close();
	readattach.close();
}

//generate end to end key while sending
char* user::generate_key_send(message ob)
{
	char plainkey[40];
	char cipherkey[40];
	strcpy_s(plainkey, ob.getreceiver());
	strcat_s(plainkey, username); //first receiver, then sender
	strcpy_s(cipherkey, plainkey);

	for (int i = 0; cipherkey[i] != '\0'; ++i)
	{
		char c = cipherkey[i];
		c += i;
		cipherkey[i] = c;
	}
	cout << "\nCipher key is:" << cipherkey << endl;
	return cipherkey;
}

//get details for signup
void user::signup()
{
	cout << "\nEnter username(max 30 charachters)";
	cin >> username;
	cout << "Enter password(8-16):";
	cin >> password;
	strcpy_s(received_file_name, username);
	strcat_s(received_file_name, "_received.dat");
}

//log in to already present account
int user::login()
{
	fstream in_userdata("user.dat", ios::in | ios::binary);
	user ob1;
	int f = 0;
	int f2 = 0;
	cout << "\nEnter username:";
	cin >> username;
	do
	{
		cout << "Enter password";
		cin >> password;
		in_userdata.seekg(0, ios::beg);
		while (in_userdata.read((char*)&ob1, sizeof(ob1)))
		{

			//cout << ob1.getusername() << ob1.getpass();
			if (!(strcmp(ob1.getusername(), username)))
			{
				//cout << "Uname done";
				if (!(strcmp(ob1.getpass(), password)))
				{
					f2 = 1; f = 1;
					cout << "\nSuccessfully logged in!";
					return 1;
				}
			}
		}

		if (f2 == 0)
		{
			int choice;
			cout << "\nIncorrect username or password!Press 1 to re-enter or 2 to return to main menu:";
			cin >> choice;
			switch (choice)
			{
			case 1: f = 0;
				break;
			case 2: return 0;
				break;
			default: cout << "\nWrong choice!Enter again:";
				f = 0;
				break;
			}
		}

	} while (f == 0);
	in_userdata.close();
	return 1;
}

//saving details onto file during signup
void user::save_details()
{
	fstream out_userdata("user.dat", ios::app | ios::binary);
	fstream in_userdata("user.dat", ios::in | ios::binary);
	user ob1;
	out_userdata.seekp(0, ios::end);
	if ((int)(out_userdata.tellg()) == 0)
		out_userdata.write((char*)this, sizeof(user));
	else
	{
		in_userdata.seekg(0, ios::beg);
		while (!in_userdata.eof())
		{
			in_userdata.read((char*)&ob1, sizeof ob1);
			if (!(strcmp(ob1.getusername(), username)))
			{
				cout << "\nUsername already exists!\n";
				return;
			}
		}
		out_userdata.write((char*)this, sizeof(user));
	}
	cout << "\nAccount successfully created.";
	out_userdata.close();
	in_userdata.close();
	return;
}

//initialize the RC4 array from 0 to 255
void initRC4()
{
	for (int i = 0; i < 256; ++i)
	{
		S[i] = i;
	}

	I = 0; J = 0;
}

//KSA scrambles up the RC4 array. After KSA array is 0 to 255 but not in order
void KSAfunc(char* key, int key_len)
{
	for (I = 0; I < 256; I++)
	{
		J = (J + S[I] + key[I % key_len]) % 256;
		swap(&S[I], &S[J]);
	}
	I = 0, J = 0; //Resetting for start of PRGA
}

//generating and returning a one-byte sized key to encrypt
char PRGAfunc()
{
	I = (I + 1) % 256;
	J = (J + S[I]) % 256;
	swap(&S[I], &S[J]);
	return S[(S[I] + S[J]) % 256];
}

//Encryption and decryption using RC4
void encrypt(char *infile, char *outfile, char *encryption_key)
{
	fstream in, out;
	int encryption_key_length = strlen((char *)encryption_key);
	char data_byte;
	char keystream_byte;
	char encrypted_byte;
	int bytes_read = 1;

	in.open(infile, ios::in);
	out.open(outfile, ios::out);

	if (encryption_key_length > 256)
	{
		cout << "Key too long! Exiting";
		return;
	}

	cout << "Initializing the RC4 Array";
	initRC4();
	cout << "Starting KSA";
	KSAfunc(encryption_key, encryption_key_length);

	cout << "Opening files and strting encryption";

	while (in.read(&data_byte, 1)) //reads file byte by beat
	{
		keystream_byte = PRGAfunc(); //generates one byte key usinhg PRGA
		encrypted_byte = data_byte ^ keystream_byte; //encrypts byte
													 //cout << endl;
													 //cout << "Data:" << data_byte << " Keystream:" << keystream_byte << " Encrypted:" << encrypted_byte;
		out.write(&encrypted_byte, 1); //writes onto new file byte by byte
	}
	cout << "\n\nENCRYPTION SUCCESFUL";

	in.close();
	out.close();
	return;
}

void switchfiles_encrypt(char* infile, char* newfile)
{
	char s[200];
	fstream in, out;
	in.open(infile, ios::in);
	out.open(newfile, ios::out);
	while (in.getline(s, 200, ' '))
	{
		out << s << ' ';
	}
	in.close();
	in.open(infile, ios::trunc);
	in.close();
	out.close();
}

//Write the plain text entered by user into file
void writefile(char *infile)
{
	fstream out;
	out.open(infile, ios::out);
	char s[200];
	cout << "Enter the text into the file:";
	cin.ignore(); gets(s);
	out << s;
}

void displayfile(char* file)
{
	char s[200];
	fstream in;
	in.open(file, ios::in);
	while (in.getline(s, 200, ' '))
	{
		cout << s << ' ';
	}
	in.close();
}


int main()
{
	char ch2, ch3, ch4;
	char choice;
	char filename[30];
	char outfile[30];
	char key[30];
	int f = 0;
	int b = 0;
	int menu;
	do {
		//system("CLS");
		cout << "\n\n\t\tWelcome to SecureComm!\n1.Create account\n2.Login\n3.Exit\nENter your choice:";
		cin >> menu;
		switch (menu)
		{
		case 1:	ob.signup();
			ob.save_details();
			break;
		case 2: f = ob.login();
			//ob.display();
			break;
		case 3: exit(0);
			break;
		default: cout << "\nWrong option!Enter again:";
			break;
		}
		if (f == 1)
		{
			//ob.display();
			do {
				//system("CLS");
				cout << "\n\n\t\t Sub menu \n1. Send message/file \n2. View received files \n3. View received files and then open one \n4. Log out \nEnter choice: ";
				cin >> ch2;
				switch (ch2)
				{
				case '1': ob.sendfile(); break;
				case '2': ob.display(); ob.display_received(); break;
				case '3': ob.openfile(); break;
				case '4': f = 0; cout << "LOGGED OUT SUCCESSULLY"; break;
				default: cout << "INVALID"; break;
				}
				if (f == 0) break;
				cout << "Go back to sub menu?"; cin >> ch3;
			} while (ch3 == 'y' || ch3 == 'Y');
		}
	} while (f == 0 || ch3 == 'y' || ch3 == 'Y');
	_getch();
	_getch();
	_getch();
}
