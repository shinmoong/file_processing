#include "subscription.h"
#include <fstream>
#include <vector>
#include "buffile.h"
#include "delim.h"
#include "iobuffer.h"
#include "recfile.h"
#include "varlen.h"

using namespace std;

int subscriptiontest() {
	ifstream ifs("listOfSubscription.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		return -1;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Subscription> SubscriptionFile(buffer);
	char file[30] = "fileOfSubscription.dat";
	SubscriptionFile.Create(file, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Subscription s;
		ifs >> s;
		int recaddr;
		if ((recaddr = SubscriptionFile.Write(s)) == -1) {
			cout << "Write Error!" << endl;
		}
	}
	SubscriptionFile.Close();

	SubscriptionFile.Open(file, ios::in);
	for (int i = 0; i < 10; i++) {
		Subscription s;
		if (SubscriptionFile.Read(s) == -1) {
			break;
		}
		cout << s;
	}
	SubscriptionFile.Close();

	return 0;
}