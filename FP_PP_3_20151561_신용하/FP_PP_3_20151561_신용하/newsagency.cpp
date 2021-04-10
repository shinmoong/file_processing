#include "newsagency.h"
#include "recfile.h"
#include <sstream>
#include <vector>

NewsAgency::NewsAgency() {
}

NewsAgency::NewsAgency(const char* new_newsagency_id, const string &new_name, const string &new_address) {
	update_newsagency_id(new_newsagency_id);
	update_name(new_name);
	update_address(new_address);
}

NewsAgency::NewsAgency(const NewsAgency & n) {
	update_newsagency_id(n.newsagency_id);
	update_name(n.name);
	update_address(n.address);
}

NewsAgency& NewsAgency::operator = (const NewsAgency &n) {

	update_newsagency_id(n.newsagency_id);
	update_name(n.name);
	update_address(n.address);

	return *this;

}

bool NewsAgency::operator == (const NewsAgency &n) {
	return !memcmp(newsagency_id, n.newsagency_id, 12);
}

bool NewsAgency::operator != (const NewsAgency &n) {
	return 0 != memcmp(newsagency_id, n.newsagency_id, 12);
}

istream& operator >> (istream &is, NewsAgency &n) {
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
	n.update_newsagency_id(token.data());
	getline(iss, token, '|');
	n.update_name(token);
	getline(iss, token, '|');
	n.update_address(token);

	return is;
}

ostream& operator << (ostream &os, NewsAgency &n) {
	string newsagency_id(n.newsagency_id, LENNID);

	os << "NEWSAGENCY ID : " << newsagency_id << endl;
	os << "NAME : " << n.name << endl;
	os << "ADDRESS : " << n.address << endl;
	return os;
}

bool NewsAgency::Pack(IOBuffer & Buffer) const {
	int numBytes;

	Buffer.Clear();

	string n_newsagency_id(newsagency_id, LENNID);
	
	numBytes = Buffer.Pack(n_newsagency_id.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(name.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(address.c_str());
	if (numBytes == -1) return false;

	return true;
}

bool NewsAgency::Unpack(IOBuffer & Buffer) {
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(newsagency_id, LENNID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	name = buf;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	address = buf;

	return true;
}

char * NewsAgency::Key() {
	key.assign(newsagency_id, LENNID);
	return (char *)(key.c_str());
}