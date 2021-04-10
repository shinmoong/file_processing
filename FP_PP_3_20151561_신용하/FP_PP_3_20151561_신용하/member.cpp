#include "member.h"
#include "recfile.h"
#include <sstream>
#include <vector>

Member::Member() {
}

Member::Member(const string &new_id, const string &new_password, const string &new_name, const string &new_phone_number, const string &new_address, const char *new_mileage, const string &new_level) {
	update_id(new_id);
	update_password(new_password);
	update_name(new_name);
	update_phone_number(new_phone_number);
	update_address(new_address);
	update_mileage(new_mileage);
	update_level(new_level);
}

Member::Member(const Member & m) {
	update_id(m.id);
	update_password(m.password);
	update_name(m.name);
	update_phone_number(m.phone_number);
	update_address(m.address);
	update_mileage(m.mileage);
	update_level(m.level);
}

Member& Member::operator = (const Member &m) {
	update_id(m.id);
	update_password(m.password);
	update_name(m.name);
	update_phone_number(m.phone_number);
	update_address(m.address);
	update_mileage(m.mileage);
	update_level(m.level);
	return *this;

}

bool Member::operator == (const Member &m) {
	if (id.compare(m.id) == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Member::operator != (const Member &m) {
	if (id.compare(m.id) == 0) {
		return false;
	}
	else {
		return true;
	}
}

istream& operator >> (istream &is, Member &m) {
	string st;

	is.exceptions(istream::failbit | istream::badbit);

	try {
		getline(is, st);
	}
	catch (istream::failure e) {
		// error
		return is;
	}

	istringstream iss(st);
	string token;

	getline(iss, token, '|');
	m.update_id(token);
	getline(iss, token, '|');
	m.update_password(token);
	getline(iss, token, '|');
	m.update_name(token);
	getline(iss, token, '|');
	m.update_phone_number(token);
	getline(iss, token, '|');
	m.update_address(token);
	getline(iss, token, '|');
	m.update_mileage(token.data());
	getline(iss, token, '|');
	m.update_level(token);
	return is;
}

ostream& operator << (ostream &os, Member &m) {
	string mileage(m.mileage, LENMIL);

	os << "ID : " << m.id << endl;
	os << "PASSWORD : " << m.password << endl;
	os << "NAME : " << m.name << endl;
	os << "PHONE NUMBER: " << m.phone_number << endl;
	os << "ADDRESS : " << m.address << endl;
	os << "MILEAGE : " << mileage << endl;
	os << "LEVEL : " << m.level << endl;
	return os;
}

bool Member::Pack(IOBuffer & Buffer) const {
	int numBytes;

	Buffer.Clear();

	string m_mileage(mileage, LENMIL);

	numBytes = Buffer.Pack(id.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(password.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(name.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(phone_number.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(address.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(m_mileage.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(level.c_str());
	if (numBytes == -1) return false;
	return true;
}

bool Member::Unpack(IOBuffer & Buffer) {
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	id = buf;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	password = buf;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	name = buf;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	phone_number = buf;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	address = buf;
	numBytes = Buffer.Unpack(mileage, LENMIL);
	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	level = buf;
	return true;
}

string Member::Key () {
	key.assign(id);
	return key;
}
