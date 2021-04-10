#pragma once
#include <iostream>
#include <string>
#include "delim.h"

#define LENMIL 10
#define STDMAXBUF 256

using namespace std;

class Member {
private:
	string id;
	string password;
	string name;
	string phone_number;
	string address;
	char mileage[10];
	string level;
	string key;
public:
	Member ();
	Member (const string &new_id, const string &new_password, const string &new_name, const string &new_phone_number, const string &new_address, const char *new_mileage, const string &new_level);
	Member (const Member & m);

	Member & operator = (const Member &m);
	bool operator == (const Member &m);
	bool operator != (const Member &m);

	friend istream & operator >> (istream &is, Member &m);
	friend ostream & operator << (ostream &os, Member &m);

	void update_id(const string new_id) { id = new_id;  }
	void update_password(const string new_password) { password = new_password; }
	void update_name(const string new_name) { name = new_name; }
	void update_phone_number(const string new_phone_number) { phone_number = new_phone_number; }
	void update_address(const string new_address) { address = new_address; }
	void update_mileage(const char *new_mileage) { memcpy(mileage, new_mileage, LENMIL); }
	void update_level(const string new_level) { level = new_level; }
	bool Pack(IOBuffer & Buffer) const;
	bool Unpack(IOBuffer &);
	string get_id(void) { return id; }
	string get_password(void) { return password; }
	string get_name(void) { return name; }
	string get_phone_number(void) { return phone_number; }
	string get_address(void) { return address; }
	char * get_mileage(void) { return mileage; }
	string get_level(void) { return level; }

	string Key ();
};


istream & operator >> (istream &is, Member &m);
ostream & operator << (ostream &is, Member &m);
