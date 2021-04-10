#pragma once
#include <iostream>
#include <string>
#include "delim.h"

#define LENNID 12
#define STDMAXBUF 256

using namespace std;

class NewsAgency {
private:
	char newsagency_id[12];
	string name;
	string address;
	string key;

public:
	NewsAgency();
	NewsAgency(const char* new_newsagency_id, const string &new_name, const string &new_address);
	NewsAgency(const NewsAgency & n);

	NewsAgency & operator = (const NewsAgency &n);
	bool operator == (const NewsAgency &n);
	bool operator != (const NewsAgency &n);

	friend istream & operator >> (istream &is, NewsAgency &n);
	friend ostream & operator << (ostream &os, NewsAgency &n);

	void update_newsagency_id(const char* new_newsagency_id) { memcpy(newsagency_id, new_newsagency_id, 12); }
	void update_name(const string new_name) { name = new_name; }
	void update_address(const string new_address) { address = new_address; }
	bool Pack(IOBuffer & Buffer) const;
	bool Unpack(IOBuffer &);
	char * get_newsagency_id(void) { return newsagency_id; }
	string get_name(void) { return name; }
	string get_address(void) { return address; }
	char * Key();
};


istream & operator >> (istream &is, NewsAgency &n);
ostream & operator << (ostream &is, NewsAgency &n);
