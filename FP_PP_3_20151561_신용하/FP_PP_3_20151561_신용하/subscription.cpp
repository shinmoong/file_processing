#include "subscription.h"
#include "recfile.h"
#include <sstream>
#include <vector>

Subscription::Subscription() {
}

Subscription::Subscription(const char* new_subscription_id, const char* new_newsagency_id, const string &new_member_id, const char* new_mileage) {
	update_subscription_id(new_subscription_id);
	update_newsagency_id(new_newsagency_id);
	update_member_id(new_subscription_id);
	update_mileage(new_mileage);
}

Subscription::Subscription(const Subscription & s) {
	update_subscription_id(s.subscription_id);
	update_newsagency_id(s.newsagency_id);
	update_member_id(s.member_id);
	update_mileage(s.mileage);
}

Subscription& Subscription::operator = (const Subscription &s) {

	update_subscription_id(s.subscription_id);
	update_newsagency_id(s.newsagency_id);
	update_member_id(s.member_id);
	update_mileage(s.mileage);

	return *this;

}

bool Subscription::operator == (const Subscription &s) {
	if ((memcmp(subscription_id, s.subscription_id, 16) == 0) && (memcmp(newsagency_id, s.newsagency_id, 12) == 0) && (member_id.compare(s.member_id) == 0)) {
		return true;
	}
	else {
		return false;
	}
}

bool Subscription::operator != (const Subscription &s) {
	if ((memcmp(subscription_id, s.subscription_id, 16) == 0) && (memcmp(newsagency_id, s.newsagency_id, 12) == 0) && (member_id.compare(s.member_id) == 0)) {
		return false;
	}
	else {
		return true;
	}
}

istream& operator >> (istream &is, Subscription &s) {
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
	s.update_subscription_id(token.data());
	getline(iss, token, '|');
	s.update_newsagency_id(token.data());
	getline(iss, token, '|');
	s.update_member_id(token);
	getline(iss, token, '|');
	s.update_mileage(token.data());

	return is;
}

ostream& operator << (ostream &os, Subscription &s) {
	string subscription_id(s.subscription_id, LENSID);
	string newsagency_id(s.newsagency_id, LENNID);
	string mileage(s.mileage, LENMIL);

	os << "SUBSCRIPTION ID : " << subscription_id << endl;
	os << "NEWSAGENCY ID : " << newsagency_id << endl;
	os << "MEMBER ID : " << s.member_id << endl;
	os << "MILEAGE : " << mileage << endl;

	return os;
}

bool Subscription::Pack(IOBuffer & Buffer) const {
	int numBytes;

	Buffer.Clear();

	string s_subscription_id(subscription_id, LENSID);
	string s_newsagency_id(newsagency_id, LENNID);
	string s_mileage(mileage, LENMIL);

	numBytes = Buffer.Pack(s_subscription_id.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(s_newsagency_id.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(member_id.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(s_mileage.c_str());
	if (numBytes == -1) return false;

	return true;
}

bool Subscription::Unpack(IOBuffer & Buffer) {
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(subscription_id, LENSID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(newsagency_id, LENNID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	member_id = buf;
	numBytes = Buffer.Unpack(mileage, LENMIL);
	if (numBytes == -1) return false;

	return true;
}

char * Subscription::Key() {
	key.assign(subscription_id, LENSID);
	return (char *)(key.c_str());
}