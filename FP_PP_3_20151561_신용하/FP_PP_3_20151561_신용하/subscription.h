#pragma once
#include <iostream>
#include <string>
#include "delim.h"

#define LENSID 16
#define LENNID 12
#define LENMIL 10
#define STDMAXBUF 256

using namespace std;

class Subscription {
private:
	char subscription_id[16];
	char newsagency_id[12];
	string member_id;
	char mileage[10];
	string key;

public:
	Subscription();
	Subscription(const char* new_subscription_id, const char* new_newsagency_id, const string &new_member_id, const char* new_mileage);
	Subscription(const Subscription & s);

	Subscription & operator = (const Subscription &s);
	bool operator == (const Subscription &s);
	bool operator != (const Subscription &s);

	friend istream & operator >> (istream &is, Subscription &s);
	friend ostream & operator << (ostream &os, Subscription &s);

	void update_subscription_id(const char* new_subscription_id) { memcpy(subscription_id, new_subscription_id, 16); }
	void update_newsagency_id(const char* new_newsagency_id) { memcpy(newsagency_id, new_newsagency_id, 12); }
	void update_member_id(const string new_member_id) { member_id = new_member_id; }
	void update_mileage(const char* new_mileage) { memcpy(mileage, new_mileage, 10); }
	bool Pack(IOBuffer & Buffer) const;
	bool Unpack(IOBuffer &);
	char* get_subscription_id(void) { return subscription_id; }
	char* get_newsagency_id(void) { return newsagency_id; }
	string get_member_id(void) { return member_id; }
	char * Key();
};


istream & operator >> (istream &is, Subscription &s);
ostream & operator << (ostream &is, Subscription &s);
