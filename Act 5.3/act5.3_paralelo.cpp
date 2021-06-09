/*
####################################
# Actividad Integradora 5.3        #
# Resaltador de sintaxis paralelo  #
# Version 1                        #
#                                  #
# Daniel Cruz Arciniega A01701370  #
# Manolo Ramirez Pintor A01706155  #
####################################
*/

//compilacion en linux: g++ -pthread act5.3_paralelo.cpp
//compilacion en otros sistemas: g++ -lpthread act5.3_paralelo.cpp

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

#define THREADS 8

class DFA {
	private:
		//string token;
		vector<string> v;
		int count; //Variable para saber cuantos elementos hay en los vectores
 		int index; //variable auxiliar en caso de que se requieran saltar índices en el ciclo for de lexerAritmetico
		
	public:
		DFA(vector<string> &vec) {v=vec; count = 0; index=0;}
		//void lexerAritmetico(string &, int, int);
		void processEntry(string &, int, int);
		bool rw(string, string, int);
		bool str(string, int);
		string var(string, int);
		bool isFloat(string, int);
		string nfloat(string, int);
		string entero(string, int);
};
/*
void DFA::lexerAritmetico(string &token, int start, int end){
	string s;

	for (int nline = start; nline < end; nline++){
		s = v[nline];
		proccesEntry(s, token);
		token += "<br>";
	}	
}
*/
//funcion para las palabras reservadas
bool DFA::rw(string s, string rws, int i){
	stringstream aux;
	int acum = -1;
	
	for(int j = i; j < rws.size()+i; j++){
		aux << s[j];
		acum++;
	}
	if(aux.str() == rws){
		index += acum;
		return true;
	}
	else
		return false;
}

//funcion para los caracteres que forman un string
bool DFA::str(string s, int i){
	if(s[i] == '"'){
		index++;
		for(int j = i+1; j < s.size(); j++){
			index ++;
			if(s[j] == '"'){
				return true;
			}
		}
	}
	else if(s[i] == '\''){
		index++;
		for(int j = i+1; j < s.size(); j++){
			index ++;
			if(s[j] == '\''){
				return true;
			}
		}
	}
	else if(s[i] == '<'){
		index++;
		for(int j = i+1; j < s.size(); j++){
			index ++;
			if(s[j] == '>'){
				return true;
			}
		}
	}
	
	return false;
}

//funcion que une todos los caracteres que forman una variable
string DFA::var(string s, int i) { 
	stringstream aux;

	for(int j = i; j < s.size(); j++){
		if(isalpha(s[j]) || s[j] == '_' || isdigit(s[j])){
			aux << s[j];
			index++;
		} else {
			break;
		}
	}
	return aux.str();
}

//funcion que verifica si un numero tiene punto flotante
bool DFA::isFloat(string s, int i){ 
	for(int j = i; j < s.size(); j++){
		if(s[j] == '.' || s[j] == 'E')
			return true;
	}
	return false;
}

//funcion que une todos los caracteres que forman un numero flotante
string DFA::nfloat(string s, int i) { 
	stringstream aux;
	
	for(int j = i; j < s.size(); j++){
		if(isdigit(s[j]) || s[j] == '.' || s[j] == 'E' || (s[j] == '-' && s[j-1] != (isdigit(s[j-1]) || ' '))){
			aux << s[j];
			index ++;
		} else {
			break;
		}
	}
	return aux.str();
}

//funcion que une todos los caracteres que forman un numero entero
string DFA::entero(string s, int i) {
	stringstream aux;
	
	for(int j = i; j < s.size(); j++){
		if(isdigit(s[j]) || (s[j] == '-' && s[j-1] != (isdigit(s[j-1]) || ' '))) {
			aux << s[j];
			index++;
		} else{
			break;
		}
	}
	return aux.str();
}

//función que recibe un string y clasifica cada caracter
void DFA::processEntry(string &token, int start, int end){
	char c;
	string s;
	
	for (int nline = start; nline < end; nline++){
		s = v[nline];
		for (int i = 0; i < s.size(); i++){
			c = s[i];
			stringstream aux;
			index = 0;
			
			//Comentarios 
			if(c == '/' && s[i+1] == '/') {
				for(int j = i; j < s.size(); j++)
					aux << s[j];
				token += "<span class='comment'>" + aux.str() + "</span>";
				count++;
				break;
			}
			
			//Palabras reservadas
			else if(isalpha(c) && rw(s, "int", i) == true){
				token += "<span class='reserved-word'> int </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "char", i) == true){
				token += "<span class='reserved-word'> char </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "float", i) == true){
				token += "<span class='reserved-word'> float </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "string", i) == true){
				token += "<span class='reserved-word'> string </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "bool", i) == true){
				token += "<span class='reserved-word'> bool </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "auto", i) == true){
				token += "<span class='reserved-word'> auto </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "long", i) == true){
				token += "<span class='reserved-word'> long </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "void", i) == true){
				token += "<span class='reserved-word'> void </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "if", i) == true){
				token += "<span class='reserved-word'> if </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "else", i) == true){
				token += "<span class='reserved-word'> else </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "while", i) == true){
				token += "<span class='reserved-word'> while </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "for", i) == true){
				token += "<span class='reserved-word'> for </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "var", i) == true){
				token += "<span class='reserved-word'> var </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "let", i) == true){
				token += "<span class='reserved-word'> let </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "const", i) == true){
				token += "<span class='reserved-word'> const </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "include", i) == true){
				token += "<span class='reserved-word'> include </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "std", i) == true){
				token += "<span class='reserved-word'> std </span>";
				count++;
				i += index;
			}
			else if(isalpha(c) && rw(s, "return", i) == true){
				token += "<span class='reserved-word'> return </span>";
				count++;
				i += index;
			}
			
			//Variables
			else if(isalpha(c)) {
				aux << c;
				aux << var(s, i+1);
				token += "<span class='var'>" + aux.str() + "</span>";
				count++;
				i += index;
			}
			
			
			//Enteros y flotantes
			else if(isdigit(c) || (c == '-' && isdigit(s[i+1]) && s[i-1] == ('('))) {
				aux << c;
				
				switch(isFloat(s, i+1)){
					case true:
						aux << nfloat(s, i+1);
						token += "<span class='float'>" + aux.str() + "</span>";
						count++;
						i += index;
						break;
					case false:
						aux << nfloat(s, i+1);
						token += "<span class='int'>" + aux.str() + "</span>";
						count++;
						i += index;
						break;
				}
			}
			
			//Strings
			else if(str(s, i) == true) {
				aux << c;
				for(int j = i+1; j < s.size(); j++){
					aux << s[j];
					if(s[j] == '"' || s[j] == '\'' || s[j] == '>')
						break;
				}
				token += "<span class='string'>" + aux.str() + "</span>";
				count ++;
				i += index;
			}
			
			//#include
			else if(c == '#') {
				for(int j = i; j < s.size(); j++){
					aux << s[j];
					index++;
					if(s[j] == ' ')
						break;
				}
				token += "<span class='include'>" + aux.str() + "</span>";
				count ++;
				i += index;
			}
			
			//Operadores
			else if(c == '=') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == ',') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '+') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '-') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '*') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '/') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '^') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '(') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == ')') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '{') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '}') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '[') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == ']') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == ';') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == ':') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '<') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '>') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '|') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '%') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '&') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '?') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '!') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			else if(c == '~') {
				aux << c;
				token += "<span class='operador'>" + aux.str() + "</span>";
				count++;
			}
			
			//Tabs
			else if(c == '\t') {
				token += "&nbsp;&nbsp;&nbsp;&nbsp;";
			}
			
			//Espacios
			else if(c == ' ') {
				token += "&nbsp;";
			}
		}
	token += "<br>";
	}
}

//funcion para escribir el archivo .html

void write(string token) {
	string head = "<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Actividad Integradora 5.3 Resaltador de sintaxis paralelo</title><link rel='stylesheet' href='css/style.css'></head><body>";
	
	ofstream escribe("prueba.html");
	if(escribe.is_open()){
		escribe << head;
		escribe << token;
		escribe << "</body></html>";
		escribe.close();
	}
	else{
		cout << "Unable to open file";
	}
}

//funcion que recibe el nombre de un archivo y lo lee si lo encuentra 
vector<string> readf(string archivo){
	string line;
	vector<string> v;
	
	ifstream lee(archivo);
	if (lee.is_open()){	
		while (getline(lee, line, '\n')){
			v.push_back(line);
		}
		
		lee.close();
	}
	else{
		cout << "Unable to open file";
    }
	
	return v;
}

int main(int argc, char* argv[]) {
	string file;
	
	cout << "Introduzca el nombre del archivo que desea cargar: ";
	getline(cin, file);
	cout << "\n";
	
	//tiempo inicial
	auto startt = chrono::steady_clock::now();
	
	string tokens[THREADS];
	vector<string> vfile = readf(file);
	int block = vfile.size()/THREADS;
	vector<int> starts, ends;
	int start, end;
	
	DFA dfa(vfile);
	
	for (int i = 0; i < THREADS; i++){
		start = i*block;
		end = (i == THREADS - 1)? vfile.size() : (i + 1) * block;
		starts.push_back(start);
		ends.push_back(end);
	}
	
	thread t1([&] { dfa.processEntry(tokens[0], starts[0], ends[0]) ; });
	thread t2([&] { dfa.processEntry(tokens[1], starts[1], ends[1]) ; });
	thread t3([&] { dfa.processEntry(tokens[2], starts[2], ends[2]) ; });
	thread t4([&] { dfa.processEntry(tokens[3], starts[3], ends[3]) ; });
	thread t5([&] { dfa.processEntry(tokens[4], starts[4], ends[4]) ; });
	thread t6([&] { dfa.processEntry(tokens[5], starts[5], ends[5]) ; });
	thread t7([&] { dfa.processEntry(tokens[6], starts[6], ends[6]) ; });
	thread t8([&] { dfa.processEntry(tokens[7], starts[7], ends[7]) ; });
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	
	stringstream ftoken;
	for (string word: tokens)
		ftoken << word;
	
	//cout << ftoken.str() << endl;
	
	write(ftoken.str());
	
	//tiempo final
	auto endt = chrono::steady_clock::now();
	
	//tiempo total
	double tiempo = double (chrono::duration_cast <chrono::nanoseconds> (endt - startt).count());
	cout << "Tiempo de ejecucion: " << tiempo/1e9 << "s" << endl;
}