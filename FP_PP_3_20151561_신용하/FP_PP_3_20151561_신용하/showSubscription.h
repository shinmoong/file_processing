#include "subscription.h"
#include <fstream>
#include <vector>
#include "buffile.h"
#include "delim.h"
#include "iobuffer.h"
#include "recfile.h"
#include "varlen.h"

int showsubscription() {
	ifstream ifs("listOfSubscription.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		return -1;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	if (n < 10) {
		for (int i = 0; i < n; i++) {
			Subscription s;
			ifs >> s;
			cout << s;
		}
		return 0;
	}
	for (int i = 0; i < 10; i++) {
		Subscription s;
		ifs >> s;
		cout << s;
	}
	return 0;
}