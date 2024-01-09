#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <string>
using namespace std;

class Struktura;

class Cvor {
	string sadrzaj, ispis;
	int oznaka;
	vector <Cvor*> potomci;
	vector<int> ocene;
	Cvor* roditelj;
	void zameni(Cvor& c1, Cvor& c2) {
		std::swap(c1.ocene, c2.ocene);
		std::swap(c1.potomci, c2.potomci);
		std::swap(c1.sadrzaj, c2.sadrzaj);
	}
	friend Struktura;
public:

	Cvor(string s) :sadrzaj(s) {};
	Cvor(const Cvor& c);
	Cvor(Cvor&& c);
	Cvor& operator=(Cvor c);
	~Cvor();
	void postaviOznaku(int i) { oznaka = i; }
	void dodajPotomka(Cvor* c);
	string dohvSadrzaj() const { return sadrzaj; }
};
Cvor::Cvor(const Cvor& c) {
	sadrzaj = c.sadrzaj;
	for (int i = 0; i < c.ocene.size(); i++) {
		ocene[i] = c.ocene[i];
	}
	for (int i = 0; i < c.potomci.size(); i++) {
		*potomci[i] = *(c.potomci[i]);
	}

}
Cvor::Cvor(Cvor&& c) {
	sadrzaj = c.sadrzaj;
	ocene = c.ocene;
	potomci = c.potomci;
	c.potomci.clear();
	c.ocene.clear();
}
Cvor& Cvor::operator=(Cvor c)
{
	zameni(*this, c);
	return *this;
}
Cvor::~Cvor()
{
	for (int i = 0; i < potomci.size(); ) {
		delete potomci[i];
		potomci.erase(potomci.begin() + i);

	}

	ocene.clear();
	sadrzaj = "";
	ispis = "";
}
class Struktura {
	vector<Cvor*> pitanja;
	void zameniIspis(Cvor* c);
public:
	Struktura& dodajPitanje(string s);
	Cvor* pretragaPitanja(string p) const;
	Struktura& dodajOdgovor(string s, string p, string od);
	Cvor* pretragaOdgovora(string s, string p) const;
	Struktura& povecanjeOcene(string p, string od, int o);
	void ispisPitanja() const;
	void ispisPitanjaIOdgovora(Cvor* p) const;
	Struktura& brisanjeOdgovora(string p, string od);
	Struktura& brisanjePitanja(string p);
	Cvor* nadjiPitanje() const;
	void sortiranje();
	~Struktura();
};
Struktura::~Struktura()
{
	for (int i = 0; i < pitanja.size(); i++) {
		delete pitanja[i];
		pitanja.erase(pitanja.begin() + i);
	}

}
void Cvor::dodajPotomka(Cvor* c)
{
	if (this) {
		int i = potomci.size();
		if (i == 10) {
			if (oznaka == 0)
				cout << "NE MOZE BITI VISE OD 10 ODGOVORA.";
			else cout << "NE MOZE BITI VISE OD 10 KOMENTARA.";
		}
		else {
			potomci.insert(potomci.begin() + i, c);
			potomci[i]->postaviOznaku(1);
			ocene.insert(ocene.begin() + i, 0);
		}
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}
Cvor* Struktura::pretragaPitanja(string p) const
{
	if (!this) return nullptr;
	int i = 0;
	while (i < pitanja.size()) {
		if (pitanja[i]->dohvSadrzaj() == p) {
			return pitanja[i];
		}
		else i++;
	}
	cout << "NE POSTOJI TRAZENO PITANJE.";
	return nullptr;
}
Cvor* Struktura::pretragaOdgovora(string s, string p) const
{
	if (!this)return nullptr;
	Cvor* k = pretragaPitanja(p);
	if (k) {
		queue<Cvor*> q;
		Cvor* next = k;
		q.push(k);
		while (!q.empty()) {
			next = q.front();
			q.pop();
			if (next->dohvSadrzaj() == s) return next;
			for (int i = 0; i < next->potomci.size(); i++) {
				q.push(next->potomci[i]);
			}
		}
	}
	cout << "NE POSTOJI TRAZENI ODGOVOR.";
	return nullptr;
}
Struktura& Struktura::dodajPitanje(string s)
{
	if (this) {
		int i = pitanja.size();
		if (i == 10) {
			cout << "NE MOZE BITI VISE OD 10 PITANJA" << endl;
		}
		else {
			pitanja.insert(pitanja.begin() + i, new Cvor(s));
			pitanja[i]->postaviOznaku(0);
			pitanja[i]->roditelj = nullptr;
			pitanja[i]->ispis = "1.";
		}
		return *this;
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}
Struktura& Struktura::dodajOdgovor(string s, string p, string od)
{
	if (this) {
		Cvor* n = pretragaPitanja(p);
		if (n) {
			if (od == "") {
				Cvor* c = new Cvor(s);
				n->dodajPotomka(c);
				c->roditelj = n;
				int i = find(n->potomci.begin(), n->potomci.end(), c) - n->potomci.begin();
				c->ispis = c->roditelj->ispis + to_string(i + 1) + ".";
			}
			else {
				Cvor* n = pretragaOdgovora(od, p);
				if (n) {
					Cvor* c = new Cvor(s);
					n->dodajPotomka(c);
					c->roditelj = n;
					int i = find(n->potomci.begin(), n->potomci.end(), c) - n->potomci.begin();
					c->ispis = c->roditelj->ispis + to_string(i + 1) + ".";
				}
			}
		}
		return *this;
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}
Struktura& Struktura::povecanjeOcene(string p, string od, int o)
{
	if (this) {
		Cvor* k = pretragaPitanja(p);
		if (k) {
			queue<Cvor*> q;
			Cvor* next = k;
			q.push(k);
			while (!q.empty()) {
				next = q.front();
				q.pop();
				for (int i = 0; i < next->potomci.size(); i++) {
					if (next->potomci[i]->dohvSadrzaj() == od) {
						next->ocene[i] += o;
						return *this;
					}
				}
				for (int i = 0; i < next->potomci.size(); i++) {
					q.push(next->potomci[i]);
				}
			}
		}
		cout << "NE POSTOJI TRAZENI ODGOVOR.";
		return *this;
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}
void Struktura::ispisPitanja() const
{
	if (this) {
		for (int i = 0; i < pitanja.size(); i++) {
			cout << pitanja[i]->dohvSadrzaj() << endl;
		}
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}
void Struktura::ispisPitanjaIOdgovora(Cvor* p) const
{
	if (this) {
		Cvor* k = p;
		if (k) {
			queue<Cvor*> q;
			Cvor* next = k;
			q.push(k);
			while (!q.empty()) {
				next = q.front();
				q.pop();
				cout << next->ispis << next->dohvSadrzaj() << endl;
				for (int i = 0; i < next->potomci.size(); i++) {
					q.push(next->potomci[i]);

				}
			}
		}
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;

}
void Struktura::sortiranje()
{
	if (this) {
		for (int i = 0; i < pitanja.size(); i++) {
			Cvor* k = pitanja[i];
			if (k) {
				queue<Cvor*> q;
				Cvor* next = k;
				q.push(k);
				while (!q.empty()) {
					next = q.front();
					q.pop();
					if (next->ocene.size()) {
						for (int j = 0; j < next->ocene.size() - 1; j++) {
							for (int k = j + 1; k < next->ocene.size(); k++) {
								if (next->ocene[k] > next->ocene[j]) {
									swap(next->ocene[k], next->ocene[j]);
									swap(next->potomci[k], next->potomci[j]);
									swap(next->potomci[k]->ispis, next->potomci[j]->ispis);
									zameniIspis(next->potomci[k]);
									zameniIspis(next->potomci[j]);
								}
							}
						}
					}
					for (int i = 0; i < next->potomci.size(); i++) {
						q.push(next->potomci[i]);
					}
				}
			}
		}
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}
Struktura& Struktura::brisanjeOdgovora(string p, string od)
{
	if (this) {
		Cvor* k = pretragaPitanja(p);
		if (k) {
			queue<Cvor*> q;
			Cvor* next = k;
			q.push(k);
			while (!q.empty()) {
				next = q.front();
				q.pop();
				for (int i = 0; i < next->potomci.size(); i++) {
					if (next->potomci[i]->dohvSadrzaj() == od) {
						delete next->potomci[i];
						next->ocene.erase(next->ocene.begin() + i);
						next->potomci.erase(next->potomci.begin() + i);
						zameniIspis(k);
						return *this;
					}
				}
				for (int i = 0; i < next->potomci.size(); i++) {
					q.push(next->potomci[i]);
				}
			}
		}
		cout << "NE POSTOJI TRAZENI ODGOVOR." << endl;
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}
Struktura& Struktura::brisanjePitanja(string p)
{
	if (this) {
		Cvor* k = pretragaPitanja(p);
		if (k) {
			queue<Cvor*> q;
			deque<Cvor*>dq;
			Cvor* next = k;
			q.push(k);
			while (!q.empty()) {
				next = q.front();
				q.pop();
				dq.push_back(next);
				for (int i = 0; i < next->potomci.size(); i++) {
					q.push(next->potomci[i]);
				}
			}
			while (!dq.empty()) {
				next = dq.back();
				dq.pop_back();
				Cvor* r = next->roditelj;
				if (r) {
					int j = r->potomci.size() - 2;
					for (int i = next->roditelj->potomci.size() - 1; i >= 0; i--) {
						delete r->potomci[i];
						r->potomci.erase(r->potomci.begin() + i);
						if (j >= 0) {
							dq.pop_back();
							j--;
						}
					}
				}
				else {
					int br;
					for (int i = 0; i < pitanja.size(); i++)
						if (pitanja[i] == k) br = i;
					delete k;
					pitanja.erase(pitanja.begin() + br);
				}
			}
		}
		return *this;
	}
	else cout << "NISTE DEFINISALI STURKTURU" << endl;
}Cvor* Struktura::nadjiPitanje() const
{
	if (!this) {
		cout << "NISTE DEFINISALI STRUKTURU." << endl;
		return nullptr;
	}
	vector<int> pom;
	for (int i = 0; i < pitanja.size(); i++) {
		int br = 0;
		Cvor* k = pitanja[i];
		if (k) {
			queue<Cvor*> q;
			Cvor* next = k;
			q.push(k);
			while (!q.empty()) {
				next = q.front();
				q.pop();
				for (int i = 0; i < next->potomci.size(); i++) {
					q.push(next->potomci[i]);
					br++;
				}
			}
		}
		pom.push_back(br);
	}
	int i = max_element(pom.begin(), pom.end()) - pom.begin();
	return pitanja[i];
}
void Struktura::zameniIspis(Cvor* c)
{
	queue<Cvor*> q;
	Cvor* next = c;
	q.push(c);
	while (!q.empty()) {
		next = q.front();
		q.pop();
		if (next != c) {
			int i = find(next->roditelj->potomci.begin(), next->roditelj->potomci.end(), next) - next->roditelj->potomci.begin();
			next->ispis = next->roditelj->ispis + to_string(i + 1) + ".";
		}
		for (int i = 0; i < next->potomci.size(); i++) {
			q.push(next->potomci[i]);
		}
	}

}
void ispisiMain() {
	cout << "ODABERITE NEKU OD OPCIJA" << endl << "1.DEFINISANJE STRUKTURE PODATAKA ZA KREIRANJE PROIZVOLJNOG BROJA PITANJA" << endl
		<< "2.DODAVANJE NOVOG PITANJA" << endl << "3.DODAVANJE ODGOVORA NA PITANJE ILI KOMENTARA NA ODGOVOR" << endl <<
		"4.PRETRAGA PITANJA" << endl << "5.PRETRAGA ODGOVORA" << endl <<
		"6.POVECANJE OCENE NEKOG ODGOVORA" << endl <<
		"7.UREDJIVANJE PO OCENAMA" << endl << "8.ISPISATI PITANJA" << endl << "9.ISPISATI PITANJE I NJEMU PRIDRUZENE ODGOVORE I KOMENTARE" << endl
		<< "10.OBRISATI ODGOVOR ILI KOMENTAR" << endl << "11.BRISANJE PITANJA" << endl << "12.PRONACI PITANJE SA NAJVISE ODGOVORA I KOMENTARA" << endl;
}
int main() {
	int i, j;
	string st, r, r1;
	Struktura* s = nullptr;
	Cvor* p = nullptr;
	while (true) {
		ispisiMain();
		cin >> i;
		switch (i) {
		case 1:
			s = new Struktura;
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 2:
			cout << "UNESITE TEKST PITANJA\n";
			cin.ignore();
			getline(cin, st);
			s->dodajPitanje(st);
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 3:
			cout << "UNETI ODGOVOR\n";
			cin.ignore();
			getline(cin, st);
			cout << "AKO ODGOVARATE NA PITANJE UNESTITE 0, U SUPROTNOM UNESITE 1\n";
			cin >> j;
			cin.ignore();
			cout << "UNESITE PITANJE NA KOJE ODGOVARATE ILI NA KOJE SE ODNOSI ODGOVOR KOJI CETE ISKOMENTARISATI\n";
			getline(cin, r);
			if (j) {
				cout << "UNESITE ODGOVOR KOJI CETE ISKOMENTARISATI" << endl;
				getline(cin, r1);
			}
			else r1 = "";
			s->dodajOdgovor(st, r, r1);
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 4:
			cout << "UNETI TEKST PITANJA KOJE SE TRAZI" << endl;
			cin.ignore();
			getline(cin, st);
			if (s->pretragaPitanja(st)) cout << "NADJENO\n";
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 5:
			cout << "UNETI PITANJE NA KOJE SE ODGOVOR ODNOSI" << endl;
			cin.ignore();
			getline(cin, st);
			cout << "UNETI ODGOVOR KOJI SE TRAZI" << endl;
			getline(cin, r);
			if (s->pretragaOdgovora(r, st)) cout << "NADJENO\n";
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 6:
			cout << "UNETI PITANJE NA KOJE SE ODGOVOR ODNOSI" << endl;
			cin.ignore();
			getline(cin, st);
			cout << "UNESITE ODGOVOR KOME SE TREBA POVECATI OCENA" << endl;
			getline(cin, r);
			cout << "UNESITE ZA KOLIKO TREBA POVECATI OCENU" << endl;
			cin >> j;
			s->povecanjeOcene(st, r, j);
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 7:
			s->sortiranje();
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 8:
			s->ispisPitanja();
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 9:
			cin.ignore();
			cout << "UNESITE PITANJE KOJE ZELITE DA ISPISETE" << endl;
			getline(cin, st);
			p = s->pretragaPitanja(st);
			s->ispisPitanjaIOdgovora(p);
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 10:
			cin.ignore();
			cout << "UNESITE PITANJE NA KOJE SE ODNOSI ODGOVOR" << endl;
			getline(cin, st);
			cout << "UNESITE ODGOVOR ILI KOMENTAR KOJI ZELITE DA IZBRISETE" << endl;
			getline(cin, r);
			s->brisanjeOdgovora(st, r);
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 11:
			cin.ignore();
			cout << "UNESITE PITANJE KOJE ZELITE DA IZBRISETE" << endl;
			getline(cin, st);
			s->brisanjePitanja(st);
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 12:
			p = s->nadjiPitanje();
			s->ispisPitanjaIOdgovora(p);
			cout << "UNESITE SLEDECU OPCIJU ILI 0 DA ZAVRSITE PROGRAM\n";
			break;
		case 0:
			delete s;
			return 0;
		}
	}
}