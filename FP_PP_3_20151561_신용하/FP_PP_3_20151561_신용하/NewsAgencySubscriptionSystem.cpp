#include "member.h"
#include "newsagency.h"
#include "subscription.h"
#include <fstream>
#include <vector>
#include "buffile.h"
#include "delim.h"
#include "iobuffer.h"
#include "recfile.h"
#include "varlen.h"
#include "fixlen.h"
#include "fixfld.h"
#include "textind.h"
#include "tindbuff.h"
#include "indfile.h"
#include "showMember.h"
#include "showNewsAgency.h"
#include "showSubscription.h"
#include "MemberTest.h"
#include "NewsAgencyTest.h"
#include "SubscriptionTest.h"
#include "btnode.h"
#include "btree.h"
#include <iostream>
using namespace std;

void make_index(void) {
	char mfile[100] = "fileOfMember.dat";
	char nfile[100] = "fileOfNewsAgency.dat";
	char sfile[100] = "fileOfSubscription.dat";
	char mind[100] = "fileOfMember.ind";
	char nind[100] = "fileOfNewsAgency.ind";
	char sind[100] = "fileOfSubscription.ind";
	int mcount, ncount, scount, curaddr;
	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);
	RecordFile <NewsAgency> NewsAgencyFile(buffer);
	RecordFile <Subscription> SubscriptionFile(buffer);
	const int BTreeSize = 4;

	mcount = 0;
	MemberFile.Open(mfile, ios::in);
	while (1) {
		Member m;
		if ((curaddr = MemberFile.Read(m)) == -1) {
			break;
		}
		if (curaddr <= -2) {
			continue;
		}
		mcount++;
	}
	MemberFile.Close();

	ncount = 0;
	NewsAgencyFile.Open(nfile, ios::in);
	while (1) {
		NewsAgency n;
		if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
			break;
		}
		if (curaddr <= -2) {
			continue;
		}
		ncount++;
	}
	NewsAgencyFile.Close();

	scount = 0;
	SubscriptionFile.Open(sfile, ios::in);
	while (1) {
		Subscription s;
		if ((curaddr = SubscriptionFile.Read(s)) == -1) {
			break;
		}
		if (curaddr <= -2) {
			continue;
		}
		scount++;
	}
	SubscriptionFile.Close();

	TextIndex MemberIndex(mcount+1);
	TextIndex NewsAgencyIndex(ncount+1);

	MemberFile.Open(mfile, ios::in);
	while (1) {
		Member m;
		if ((curaddr = MemberFile.Read(m)) == -1) {
			break;
		}
		if (curaddr <= -2) {
			continue;
		}
		MemberIndex.Insert(m.Key().c_str(), curaddr);
	}
	MemberFile.Close();

	NewsAgencyFile.Open(nfile, ios::in);
	while (1) {
		NewsAgency n;
		if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
			break;
		}
		if (curaddr <= -2) {
			continue;
		}
		NewsAgencyIndex.Insert(n.Key(), curaddr);
	}
	NewsAgencyFile.Close();

	TextIndexBuffer midxbuf(10, mcount+1);
	TextIndexBuffer nidxbuf(LENNID, ncount + 1);
	BufferFile MemberIdxFile(midxbuf);
	BufferFile NewsAgencyIdxFile(nidxbuf);

	MemberIdxFile.Create(mind, ios::out | ios::trunc);
	MemberIdxFile.Rewind();
	midxbuf.Pack(MemberIndex);
	MemberIdxFile.Write();
	MemberIdxFile.Close();
	NewsAgencyIdxFile.Create(nind, ios::out | ios::trunc);
	NewsAgencyIdxFile.Rewind();
	nidxbuf.Pack(NewsAgencyIndex);
	NewsAgencyIdxFile.Write();
	NewsAgencyIdxFile.Close();

	int result, i = 0;
	BTree <char> bt(BTreeSize);
	result = bt.Create(sind, ios::in | ios::out | ios::trunc);
	if (!result) { cout << "Please delete fileOfSubscription.ind" << endl; return; }
	
	SubscriptionFile.Open(sfile, ios::in);
	while (1) {
		Subscription s;
		if ((curaddr = SubscriptionFile.Read(s)) == -1) {
			break;
		}
		if (curaddr <= -2) {
			continue;
		}
		result = bt.Insert(s.Key()[0], i);
		i++;
	}
	SubscriptionFile.Close();
}

int main() {
	TextIndexBuffer x(10, 10);
	string str;
	ifstream mifs("listOfMember.txt");
	ifstream nifs("listOfNewsAgency.txt");
	ifstream sifs("listOfSubscription.txt");
	char mfile[100] = "fileOfMember.dat";
	char nfile[100] = "fileOfNewsAgency.dat";
	char sfile[100] = "fileOfSubscription.dat";
	char mind[100] = "fileOfMember.ind";
	char nind[100] = "fileOfNewsAgency.ind";
	char sind[100] = "fileOfSubscription.ind";
	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);
	RecordFile <NewsAgency> NewsAgencyFile(buffer);
	RecordFile <Subscription> SubscriptionFile(buffer);
	string login_level;
	string login_id;

	while (1) {
		cout << "Here is Home (Use: Enter command number)" << endl;
		cout << "1. showMember" << endl;
		cout << "2. showNewsAgency" << endl;
		cout << "3. showSubscription" << endl;
		cout << "4. MemberTest" << endl;
		cout << "5. NewsAgencyTest" << endl;
		cout << "6. SubscriptionTest" << endl;
		cout << "7. NewsAgencySubscriptionSystem" << endl;
		cout << "8. Exit" << endl;

		int i = 0;
		cin >> i;
		if (i == 1) {
			showmember();
		}
		else if (i == 2) {
			shownewsagency();
		}
		else if (i == 3) {
			showsubscription();
		}
		else if (i == 4) {
			membertest();
		}
		else if (i == 5) {
			newsagencytest();
		}
		else if (i == 6) {
			subscriptiontest();
		}
		else if (i == 7) {
			make_index();
			cin.ignore(256, '\n');
			cout << "Login menu" << endl;
			while (1) {
				int flag = 0;
				cout << "ID: ";
				getline(cin, str);
				MemberFile.Open(mfile, ios::in);
				Member mtmp;
				mtmp.update_id(str);
				int curaddr;
				while (1) {
					Member m;
					if ((curaddr = MemberFile.Read(m)) == -1) {
						cout << "Not correct ID!" << endl;
						break;
					}
					if (curaddr <= -2) {
						continue;
					}
					if (m == mtmp) {
						cout << "Password: ";
						getline(cin, str);
						if (str == m.get_password()) {
							login_level = m.get_level();
							login_id = m.get_id();
							flag = 1;
						}
						else {
							cout << "Not correct password!" << endl;
						}
						break;
					}
				}
				MemberFile.Close();
				if (flag == 1) {
					break;
				}
			}
			if (login_level == "1") {
				while (1) {
					make_index();
					cout << "Here is NewsAgencySubscriptionSystem" << endl;
					cout << "1. Search" << endl;
					cout << "2. Insert" << endl;
					cout << "3. Delete" << endl;
					cout << "4. Update" << endl;
					cout << "5. Go to Home" << endl;

					int j = 0;
					cin >> j;
					if (j == 1) {
						cout << "Here is Search" << endl;
						cout << "1. Search Member" << endl;
						cout << "2. Search NewsAgency" << endl;
						cout << "3. Search Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;
						if (k == 1) {
							cout << "ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							MemberFile.Open(mfile, ios::in);
							Member mtmp;
							mtmp.update_id(str);
							int curaddr;
							while (1) {
								Member m;
								if ((curaddr = MemberFile.Read(m)) == -1) {
									cout << "Not found!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (m == mtmp) {
									cout << "Find!" << endl;
									cout << m;
									break;
								}
							}
							MemberFile.Close();
						}
						else if (k == 2) {
							cout << "NewsAgency ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							if (str.length() != 12) {
								cout << "Invalid NewsAgency ID!" << endl;
								continue;
							}
							NewsAgencyFile.Open(nfile, ios::in);
							NewsAgency ntmp;
							ntmp.update_newsagency_id(str.c_str());
							int curaddr;
							while (1) {
								NewsAgency n;
								if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
									cout << "Not found!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (n == ntmp) {
									cout << "Find!" << endl;
									cout << n;
									break;
								}
							}
							NewsAgencyFile.Close();
						}
						else if (k == 3) {
							cout << "Select ID to find Subscription" << endl;
							cout << "1. Subscription ID" << endl;
							cout << "2. NewsAgency ID" << endl;
							cout << "3. Member ID" << endl;
							cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

							int l = 0;
							cin >> l;
							if (l == 1) {
								cout << "Subscription ID: ";
								cin.ignore(256, '\n');
								getline(cin, str);
								if (str.length() != 16) {
									cout << "Invalid ID!" << endl;
									continue;
								}
								SubscriptionFile.Open(sfile, ios::in);
								Subscription stmp;
								stmp.update_subscription_id(str.c_str());
								int curaddr;
								while (1) {
									Subscription s;
									if ((curaddr = SubscriptionFile.Read(s)) == -1) {
										cout << "End!" << endl;
										break;
									}
									if (curaddr <= -2) {
										continue;
									}
									stmp.update_newsagency_id(s.get_newsagency_id());
									stmp.update_member_id(s.get_member_id());
									if (s == stmp) {
										cout << "Find!" << endl;
										cout << s;
										break;
									}
								}
								SubscriptionFile.Close();
							}
							else if (l == 2) {
								cout << "NewsAgency ID: ";
								cin.ignore(256, '\n');
								getline(cin, str);
								if (str.length() != 12) {
									cout << "Invalid ID!" << endl;
									continue;
								}
								SubscriptionFile.Open(sfile, ios::in);
								Subscription stmp;
								stmp.update_newsagency_id(str.c_str());
								int curaddr;
								while (1) {
									Subscription s;
									if ((curaddr = SubscriptionFile.Read(s)) == -1) {
										cout << "End!" << endl;
										break;
									}
									if (curaddr <= -2) {
										continue;
									}
									stmp.update_subscription_id(s.get_subscription_id());
									stmp.update_member_id(s.get_member_id());
									if (s == stmp) {
										cout << "Find!" << endl;
										cout << s;
										continue;
									}
								}
								SubscriptionFile.Close();
							}
							else if (l == 3) {
								cout << "Member ID: ";
								cin.ignore(256, '\n');
								getline(cin, str);
								SubscriptionFile.Open(sfile, ios::in);
								Subscription stmp;
								stmp.update_member_id(str);
								int curaddr;
								while (1) {
									Subscription s;
									if ((curaddr = SubscriptionFile.Read(s)) == -1) {
										cout << "End!" << endl;
										break;
									}
									if (curaddr <= -2) {
										continue;
									}
									stmp.update_newsagency_id(s.get_newsagency_id());
									stmp.update_subscription_id(s.get_subscription_id());
									if (s == stmp) {
										cout << "Find!" << endl;
										cout << s;
										continue;
									}
								}
								SubscriptionFile.Close();
							}
							else {
								cin.clear();
								cin.ignore(256, '\n');
							}
						}
					}
					else if (j == 2) {
						cout << "Here is Insert" << endl;
						cout << "메뉴를 입력해 주세요." << endl;
						cout << "1. Insert Member" << endl;
						cout << "2. Insert NewsAgency" << endl;
						cout << "3. Insert Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;
						if (k == 1) {
							cout << "ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							MemberFile.Open(mfile, ios::in);
							Member mtmp;
							mtmp.update_id(str);

							int flag = 0;
							int curaddr;
							while (1) {
								Member m;
								if ((curaddr = MemberFile.Read(m)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (m == mtmp) {
									cout << "Duplicate!" << endl;
									flag = 1;
									break;
								}
							}
							MemberFile.Close();
							if (flag == 1) {
								continue;
							}
							cout << "Password: ";
							getline(cin, str);
							mtmp.update_password(str);
							cout << "Name: ";
							getline(cin, str);
							mtmp.update_name(str);
							cout << "Phone Number: ";
							getline(cin, str);
							mtmp.update_phone_number(str);
							cout << "Address: ";
							getline(cin, str);
							mtmp.update_address(str);
							cout << "Mileage: ";
							getline(cin, str);
							if (str.length() != 10) {
								cout << "Invalid length!" << endl;
								continue;
							}
							mtmp.update_mileage(str.c_str());
							mtmp.update_level("9");
							MemberFile.Open(mfile, ios::out);
							MemberFile.Append(mtmp);
							MemberFile.Close();
						}
						else if (k == 2) {
							cout << "NewsAgency ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							if (str.length() != 12) {
								cout << "Invalid length!" << endl;
								continue;
							}
							NewsAgencyFile.Open(nfile, ios::in);
							NewsAgency ntmp;
							ntmp.update_newsagency_id(str.c_str());

							int flag = 0;
							int curaddr;
							while (1) {
								NewsAgency n;
								if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (n == ntmp) {
									cout << "Duplicate!" << endl;
									flag = 1;
									break;
								}
							}
							NewsAgencyFile.Close();
							if (flag == 1) {
								continue;
							}
							cout << "Name: ";
							getline(cin, str);
							ntmp.update_name(str);
							cout << "Addreess: ";
							getline(cin, str);
							ntmp.update_address(str);
							NewsAgencyFile.Open(nfile, ios::out);
							NewsAgencyFile.Append(ntmp);
							NewsAgencyFile.Close();
						}
						else if (k == 3) {
							cout << "Subscription ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);

							int scount = 0;
							int curaddr = 0;
							SubscriptionFile.Open(sfile, ios::in);
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								scount++;
							}
							SubscriptionFile.Close();
							if (scount >= 100) {
								cout << "Subscription list full!" << endl;
								continue;
							}

							SubscriptionFile.Open(sfile, ios::in);
							Subscription stmp;
							stmp.update_subscription_id(str.c_str());

							int flag = 0;
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (memcmp(s.get_subscription_id(), stmp.get_subscription_id(), 16) == 0) {
									cout << "Duplicate!" << endl;
									flag = 1;
									break;
								}
							}
							SubscriptionFile.Close();
							if (flag == 1) {
								continue;
							}
							cout << "NewsAgency ID: ";
							getline(cin, str);
							if (str.length() != 12) {
								cout << "Invalid length!" << endl;
							}
							NewsAgencyFile.Open(nfile, ios::in);
							NewsAgency ntmp;
							ntmp.update_newsagency_id(str.c_str());

							flag = 0;
							while (1) {
								NewsAgency n;
								if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
									flag = 1;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (n == ntmp) {
									break;
								}
							}
							NewsAgencyFile.Close();
							if (flag == 1) {
								cout << "Not exist NewsAgency ID!" << endl;
								continue;
							}
							stmp.update_newsagency_id(str.c_str());
							cout << "Member ID: ";
							getline(cin, str);
							MemberFile.Open(mfile, ios::in);
							Member mtmp;
							mtmp.update_id(str);

							flag = 0;
							while (1) {
								Member m;
								if ((curaddr = MemberFile.Read(m)) == -1) {
									flag = 1;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (m == mtmp) {
									break;
								}
							}
							MemberFile.Close();
							if (flag == 1) {
								cout << "Not exist Member ID!" << endl;
								continue;
							}
							stmp.update_member_id(str);
							cout << "Mileage: ";
							getline(cin, str);
							if (str.length() != 10) {
								cout << "Invalid length!" << endl;
							}
							stmp.update_mileage(str.c_str());
							SubscriptionFile.Open(sfile, ios::out);
							SubscriptionFile.Append(stmp);
							SubscriptionFile.Close();
						}
						else {
							cin.clear();
							cin.ignore(256, '\n');
						}
					}
					else if (j == 3) {
						cout << "Here is Delete" << endl;
						cout << "메뉴를 입력해 주세요." << endl;
						cout << "1. Delete Member" << endl;
						cout << "2. Delete NewsAgency" << endl;
						cout << "3. Delete Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;
						if (k == 1) {
							cout << "ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							MemberFile.Open(mfile, ios::out);
							Member mtmp;
							mtmp.update_id(str);

							int curaddr;
							while (1) {
								Member m;
								if ((curaddr = MemberFile.Read(m)) == -1) {
									cout << "Not exist ID!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (m == mtmp) {
									MemberFile.Delete(curaddr);
									break;
								}
							}
							MemberFile.Close();

							SubscriptionFile.Open(sfile, ios::out);
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (s.get_member_id().compare(str) == 0) {
									SubscriptionFile.Delete(curaddr);
								}
							}
							SubscriptionFile.Close();
						}
						else if (k == 2) {
							cout << "NewsAgency ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							if (str.length() != 12) {
								cout << "Invalid length!" << endl;
								continue;
							}
							NewsAgencyFile.Open(nfile, ios::out);
							NewsAgency ntmp;
							ntmp.update_newsagency_id(str.c_str());

							int curaddr;
							while (1) {
								NewsAgency n;
								if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (n == ntmp) {
									NewsAgencyFile.Delete(curaddr);
									break;
								}
							}
							NewsAgencyFile.Close();

							SubscriptionFile.Open(sfile, ios::out);
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (memcmp(s.get_newsagency_id(), str.c_str(), 12) == 0) {
									SubscriptionFile.Delete(curaddr);
								}
							}
							SubscriptionFile.Close();
						}
						else if (k == 3) {
							cout << "Subscription ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							SubscriptionFile.Open(sfile, ios::out);
							Subscription stmp;
							stmp.update_subscription_id(str.c_str());

							int curaddr;
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (memcmp(s.get_subscription_id(), stmp.get_subscription_id(), 16) == 0) {
									SubscriptionFile.Delete(curaddr);
									break;
								}
							}
							SubscriptionFile.Close();
						}
						else {
							cin.clear();
							cin.ignore(256, '\n');
						}
					}
					else if (j == 4) {
						cout << "Here is Update" << endl;
						cout << "메뉴를 입력해 주세요." << endl;
						cout << "1. Update Member" << endl;
						cout << "2. Update NewsAgency" << endl;
						cout << "3. Update Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;
						if (k == 1) {
							cout << "ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							MemberFile.Open(mfile, ios::out);
							Member mtmp;
							mtmp.update_id(str);

							int curaddr;
							while (1) {
								Member m;
								if ((curaddr = MemberFile.Read(m)) == -1) {
									cout << "Cannot update!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (m == mtmp) {
									cout << "Password: ";
									getline(cin, str);
									mtmp.update_password(str);
									cout << "Name: ";
									getline(cin, str);
									mtmp.update_name(str);
									cout << "Phone Number: ";
									getline(cin, str);
									mtmp.update_phone_number(str);
									cout << "Address: ";
									getline(cin, str);
									mtmp.update_address(str);
									cout << "Mileage: ";
									getline(cin, str);
									if (str.length() != 10) {
										cout << "Invalid length!" << endl;
										break;
									}
									mtmp.update_mileage(str.c_str());
									mtmp.update_level(m.get_level());
									int tmp1 = m.get_id().length() + m.get_password().length() + m.get_name().length() + m.get_phone_number().length() + m.get_address().length();
									int tmp2 = mtmp.get_id().length() + mtmp.get_password().length() + mtmp.get_name().length() + mtmp.get_phone_number().length() + mtmp.get_address().length();
									if (tmp1 == tmp2) {
										if (curaddr <= -2) {
											MemberFile.Write(mtmp, -curaddr - 2);
										}
										else {
											MemberFile.Write(mtmp, curaddr);
										}
										break;
									}
								}
							}
							MemberFile.Close();
						}
						else if (k == 2) {
							cout << "NewsAgency ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							if (str.length() != 12) {
								cout << "Invalid length!" << endl;
								continue;
							}
							NewsAgencyFile.Open(nfile, ios::out);
							NewsAgency ntmp;
							ntmp.update_newsagency_id(str.c_str());

							int curaddr;
							while (1) {
								NewsAgency n;
								if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
									cout << "Cannot update!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (n == ntmp) {
									cout << "Name: ";
									getline(cin, str);
									ntmp.update_name(str);
									cout << "Address: ";
									getline(cin, str);
									ntmp.update_address(str);
									int tmp1 = n.get_name().length() + n.get_address().length();
									int tmp2 = n.get_name().length() + n.get_address().length();
									if (tmp1 == tmp2) {
										if (curaddr <= -2) {
											NewsAgencyFile.Write(ntmp, -curaddr - 2);
										}
										else {
											NewsAgencyFile.Write(ntmp, curaddr);
										}
										break;
									}
								}
							}
							NewsAgencyFile.Close();
						}
						else if (k == 3) {
							cout << "Subscription ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							if (str.length() != 16) {
								cout << "Invalid length!" << endl;
								continue;
							}
							SubscriptionFile.Open(sfile, ios::out);
							Subscription stmp;
							stmp.update_subscription_id(str.c_str());

							int curaddr;
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									cout << "Cannot update!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (memcmp(s.get_subscription_id(), stmp.get_subscription_id(), 16) == 0) {
									cout << "NewsAgency ID: ";
									getline(cin, str);
									if (str.length() != 12) {
										cout << "Invalid length!" << endl;
										break;
									}
									NewsAgency ntmp;
									ntmp.update_newsagency_id(str.c_str());
									int flag = 0;
									int curaddr2;
									NewsAgencyFile.Open(nfile, ios::in);
									while (1) {
										NewsAgency n;
										if ((curaddr2 = NewsAgencyFile.Read(n)) == -1) {
											flag = 1;
											cout << "Not exist NewsAgency ID!" << endl;
											break;
										}
										if (curaddr2 <= -2) {
											continue;
										}
										if (memcmp(n.get_newsagency_id(), ntmp.get_newsagency_id(), 12) == 0) {
											break;
										}
									}
									NewsAgencyFile.Close();
									if (flag == 1) {
										break;
									}
									stmp.update_newsagency_id(str.c_str());
									cout << "Member ID: ";
									getline(cin, str);
									Member mtmp;
									mtmp.update_id(str);
									flag = 0;
									MemberFile.Open(mfile, ios::in);
									while (1) {
										Member m;
										if ((curaddr2 = MemberFile.Read(m)) == -1) {
											flag = 1;
											cout << "Not exist Member ID!" << endl;
											break;
										}
										if (curaddr2 <= -2) {
											continue;
										}
										if (m.get_id().compare(mtmp.get_id()) == 0) {
											break;
										}
									}
									MemberFile.Close();
									if (flag == 1) {
										break;
									}
									stmp.update_member_id(str);
									cout << "Mileage: ";
									getline(cin, str);
									if (str.length() != 10) {
										cout << "Invalid length!" << endl;
										break;
									}
									stmp.update_mileage(str.c_str());
									int tmp1 = s.get_member_id().length();
									int tmp2 = stmp.get_member_id().length();
									if (tmp1 == tmp2) {
										if (curaddr <= -2) {
											SubscriptionFile.Write(stmp, -curaddr - 2);
										}
										else {
											SubscriptionFile.Write(stmp, curaddr);
										}
										break;
									}
								}
							}
							SubscriptionFile.Close();
						}
						else {
							cin.clear();
							cin.ignore(256, '\n');
						}
					}
					else if (j == 5) {
						break;
					}
					else {
						cin.clear();
						cin.ignore(256, '\n');
					}
				}
			}
			else if(login_level == "9"){
				while (1) {
					make_index();
					cout << "Here is NewsAgencySubscriptionSystem" << endl;
					cout << "1. Search" << endl;
					cout << "2. Insert" << endl;
					cout << "3. Delete" << endl;
					cout << "4. Update" << endl;
					cout << "5. Go to Home" << endl;

					int j = 0;
					cin >> j;
					if (j == 1) {
						cout << "Here is Search" << endl;
						cout << "1. Search NewsAgency" << endl;
						cout << "2. Search Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;

						if (k == 1) {
							cout << "NewsAgency ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							if (str.length() != 12) {
								cout << "Invalid NewsAgency ID!" << endl;
								continue;
							}
							NewsAgencyFile.Open(nfile, ios::in);
							NewsAgency ntmp;
							ntmp.update_newsagency_id(str.c_str());
							int curaddr;
							while (1) {
								NewsAgency n;
								if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
									cout << "Not found!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (n == ntmp) {
									cout << "Find!" << endl;
									cout << n;
									break;
								}
							}
							NewsAgencyFile.Close();
						}
						else if (k == 2) {
							cout << "Select ID to find Subscription" << endl;
							cout << "1. Subscription ID" << endl;
							cout << "2. NewsAgency ID" << endl;
							cout << "3. All" << endl;
							cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

							int l = 0;
							cin >> l;
							if (l == 1) {
								cout << "Subscription ID: ";
								cin.ignore(256, '\n');
								getline(cin, str);
								if (str.length() != 16) {
									cout << "Invalid ID!" << endl;
									continue;
								}
								SubscriptionFile.Open(sfile, ios::in);
								Subscription stmp;
								stmp.update_subscription_id(str.c_str());
								int curaddr;
								while (1) {
									Subscription s;
									if ((curaddr = SubscriptionFile.Read(s)) == -1) {
										cout << "End!" << endl;
										break;
									}
									if (curaddr <= -2) {
										continue;
									}
									stmp.update_newsagency_id(s.get_newsagency_id());
									stmp.update_member_id(s.get_member_id());
									if (s == stmp && s.get_member_id() == login_id) {
										cout << "Find!" << endl;
										cout << s;
										break;
									}
								}
								SubscriptionFile.Close();
							}
							else if (l == 2) {
								cout << "NewsAgency ID: ";
								cin.ignore(256, '\n');
								getline(cin, str);
								if (str.length() != 12) {
									cout << "Invalid ID!" << endl;
									continue;
								}
								SubscriptionFile.Open(sfile, ios::in);
								Subscription stmp;
								stmp.update_newsagency_id(str.c_str());
								int curaddr;
								while (1) {
									Subscription s;
									if ((curaddr = SubscriptionFile.Read(s)) == -1) {
										cout << "End!" << endl;
										break;
									}
									if (curaddr <= -2) {
										continue;
									}
									stmp.update_subscription_id(s.get_subscription_id());
									stmp.update_member_id(s.get_member_id());
									if (s == stmp && s.get_member_id() == login_id) {
										cout << "Find!" << endl;
										cout << s;
										continue;
									}
								}
								SubscriptionFile.Close();
							}
							else if (l == 3) {
								SubscriptionFile.Open(sfile, ios::in);
								Subscription stmp;
								stmp.update_member_id(login_id);
								int curaddr;
								while (1) {
									Subscription s;
									if ((curaddr = SubscriptionFile.Read(s)) == -1) {
										cout << "End!" << endl;
										break;
									}
									if (curaddr <= -2) {
										continue;
									}
									stmp.update_newsagency_id(s.get_newsagency_id());
									stmp.update_subscription_id(s.get_subscription_id());
									if (s == stmp) {
										cout << "Find!" << endl;
										cout << s;
										continue;
									}
								}
								SubscriptionFile.Close();
							}
							else {
								cin.clear();
								cin.ignore(256, '\n');
							}
						}
					}
					else if (j == 2) {
						cout << "Here is Insert" << endl;
						cout << "메뉴를 입력해 주세요." << endl;
						cout << "1. Insert Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;
						
						if (k == 1) {
							cout << "Subscription ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);

							int scount = 0;
							int curaddr = 0;
							SubscriptionFile.Open(sfile, ios::in);
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								scount++;
							}
							SubscriptionFile.Close();
							if (scount >= 100) {
								cout << "Subscription list full!" << endl;
								continue;
							}

							SubscriptionFile.Open(sfile, ios::in);
							Subscription stmp;
							stmp.update_subscription_id(str.c_str());

							int flag = 0;
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (memcmp(s.get_subscription_id(), stmp.get_subscription_id(), 16) == 0) {
									cout << "Duplicate!" << endl;
									flag = 1;
									break;
								}
							}
							SubscriptionFile.Close();
							if (flag == 1) {
								continue;
							}
							cout << "NewsAgency ID: ";
							getline(cin, str);
							if (str.length() != 12) {
								cout << "Invalid length!" << endl;
								continue;
							}
							NewsAgencyFile.Open(nfile, ios::in);
							NewsAgency ntmp;
							ntmp.update_newsagency_id(str.c_str());

							flag = 0;
							while (1) {
								NewsAgency n;
								if ((curaddr = NewsAgencyFile.Read(n)) == -1) {
									flag = 1;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (n == ntmp) {
									break;
								}
							}
							NewsAgencyFile.Close();
							if (flag == 1) {
								cout << "Not exist NewsAgency ID!" << endl;
								continue;
							}
							stmp.update_newsagency_id(str.c_str());
							stmp.update_member_id(login_id);
							cout << "Mileage: ";
							getline(cin, str);
							if (str.length() != 10) {
								cout << "Invalid length!" << endl;
								continue;
							}
							stmp.update_mileage(str.c_str());
							SubscriptionFile.Open(sfile, ios::out);
							SubscriptionFile.Append(stmp);
							SubscriptionFile.Close();
						}
						else {
							cin.clear();
							cin.ignore(256, '\n');
						}
					}
					else if (j == 3) {
						cout << "Here is Delete" << endl;
						cout << "메뉴를 입력해 주세요." << endl;
						cout << "1. Withdraw" << endl;
						cout << "2. Delete Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;
						if (k == 1) {
							MemberFile.Open(mfile, ios::out);
							Member mtmp;
							mtmp.update_id(login_id);

							int curaddr;
							while (1) {
								Member m;
								if ((curaddr = MemberFile.Read(m)) == -1) {
									cout << "Not exist ID!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (m == mtmp) {
									MemberFile.Delete(curaddr);
									break;
								}
							}
							MemberFile.Close();

							SubscriptionFile.Open(sfile, ios::out);
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (s.get_member_id().compare(login_id) == 0) {
									SubscriptionFile.Delete(curaddr);
									continue;
								}
							}
							cout << "Withdraw complete!" << endl;
							SubscriptionFile.Close();
							make_index();
							break;
						}
						else if (k == 2) {
							cout << "Subscription ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							SubscriptionFile.Open(sfile, ios::out);
							Subscription stmp;
							stmp.update_subscription_id(str.c_str());

							int curaddr;
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									cout << "There is no subscription ID!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (memcmp(s.get_subscription_id(), stmp.get_subscription_id(), 16) == 0 && s.get_member_id() == login_id) {
									SubscriptionFile.Delete(curaddr);
									break;
								}
							}
							SubscriptionFile.Close();
						}
						else {
							cin.clear();
							cin.ignore(256, '\n');
						}
					}
					else if (j == 4) {
						cout << "Here is Update" << endl;
						cout << "메뉴를 입력해 주세요." << endl;
						cout << "1. Update Member" << endl;
						cout << "2. Update Subscription" << endl;
						cout << "(Others). Go to NewsAgencySubscriptionSystem" << endl;

						int k = 0;
						cin >> k;
						if (k == 1) {
							MemberFile.Open(mfile, ios::out);
							Member mtmp;
							mtmp.update_id(login_id);

							int curaddr;
							while (1) {
								Member m;
								if ((curaddr = MemberFile.Read(m)) == -1) {
									cout << "Cannot update!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (m == mtmp) {
									cout << "Password: ";
									getline(cin, str);
									mtmp.update_password(str);
									cout << "Name: ";
									getline(cin, str);
									mtmp.update_name(str);
									cout << "Phone Number: ";
									getline(cin, str);
									mtmp.update_phone_number(str);
									cout << "Address: ";
									getline(cin, str);
									mtmp.update_address(str);
									cout << "Mileage: ";
									getline(cin, str);
									if (str.length() != 10) {
										cout << "Invalid length!" << endl;
										break;
									}
									mtmp.update_mileage(str.c_str());
									mtmp.update_level(m.get_level());
									int tmp1 = m.get_id().length() + m.get_password().length() + m.get_name().length() + m.get_phone_number().length() + m.get_address().length();
									int tmp2 = mtmp.get_id().length() + mtmp.get_password().length() + mtmp.get_name().length() + mtmp.get_phone_number().length() + mtmp.get_address().length();
									if (tmp1 == tmp2) {
										if (curaddr <= -2) {
											MemberFile.Write(mtmp, -curaddr - 2);
										}
										else {
											MemberFile.Write(mtmp, curaddr);
										}
										break;
									}
								}
							}
							MemberFile.Close();
						}
						else if (k == 2) {
							cout << "Subscription ID: ";
							cin.ignore(256, '\n');
							getline(cin, str);
							if (str.length() != 16) {
								cout << "Invalid length!" << endl;
								continue;
							}
							SubscriptionFile.Open(sfile, ios::out);
							Subscription stmp;
							stmp.update_subscription_id(str.c_str());

							int curaddr;
							while (1) {
								Subscription s;
								if ((curaddr = SubscriptionFile.Read(s)) == -1) {
									cout << "Cannot update!" << endl;
									break;
								}
								if (curaddr <= -2) {
									continue;
								}
								if (memcmp(s.get_subscription_id(), stmp.get_subscription_id(), 16) == 0 && s.get_member_id() == login_id) {
									cout << "NewsAgency ID: ";
									getline(cin, str);
									if (str.length() != 12) {
										cout << "Invalid length!" << endl;
										break;
									}
									NewsAgency ntmp;
									ntmp.update_newsagency_id(str.c_str());
									int flag = 0;
									int curaddr2;
									NewsAgencyFile.Open(nfile, ios::in);
									while (1) {
										NewsAgency n;
										if ((curaddr2 = NewsAgencyFile.Read(n)) == -1) {
											flag = 1;
											cout << "Not exist NewsAgency ID!" << endl;
											break;
										}
										if (curaddr2 <= -2) {
											continue;
										}
										if (memcmp(n.get_newsagency_id(), ntmp.get_newsagency_id(), 12) == 0) {
											break;
										}
									}
									NewsAgencyFile.Close();
									if (flag == 1) {
										break;
									}
									stmp.update_newsagency_id(str.c_str());
									stmp.update_member_id(login_id);
									cout << "Mileage: ";
									getline(cin, str);
									if (str.length() != 10) {
										cout << "Invalid length!" << endl;
										break;
									}
									stmp.update_mileage(str.c_str());
									int tmp1 = s.get_member_id().length();
									int tmp2 = stmp.get_member_id().length();
									if (tmp1 == tmp2) {
										if (curaddr <= -2) {
											SubscriptionFile.Write(stmp, -curaddr - 2);
										}
										else {
											SubscriptionFile.Write(stmp, curaddr);
										}
										break;
									}
								}
							}
							SubscriptionFile.Close();
						}
						else {
							cin.clear();
							cin.ignore(256, '\n');
						}
					}
					else if (j == 5) {
						break;
					}
					else {
						cin.clear();
						cin.ignore(256, '\n');
					}
				}
			}
		}
		else if (i == 8) {
			return 0;
		}
		else {
			cin.clear();
			cin.ignore(256, '\n');
		}
	}
}