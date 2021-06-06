/*
####################################
# Actividad Integradora 5.3        #
# Resaltador de sintaxis paralelo  #
# Version 1                        #
#                                  #
# Daniel Cruz Arciniega A01701370  #
# Manolo Ramirez Pintor            #
####################################
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <fstream>
#include <vector>
#include <chrono> 

using namespace std;

class DFA {
	private:
		string token; 
		int count; //Variable para saber cuantos elementos hay en los vectores
 		int index; //variable auxiliar en caso de que se requieran saltar índices en el ciclo for de lexerAritmetico
		
	public:
		DFA() {token=""; count = 0; index=0;}
		//bool block(string);
		void proccesEntry(string);
		void lexerAritmetico(string);
		bool rw(string, string, int);
		bool str(string, int);
		string var(string, int);
		bool isFloat(string, int);
		string nfloat(string, int);
		string entero(string, int);
		string toString();
		void write();
};

//fucnion para los comentarios de bloque

//funcion para las palabras reservadas
bool DFA::rw(string s, string rws, int i){
	stringstream aux;
	int acum = 0;
	
	for(int j = i; j < rws.size(); j++){
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
	int acum;
	
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
void DFA::proccesEntry(string s){
	char c;
	
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
		else if(rw(s, "int", i) == true){
				token += "<span class='reserved-word'> int </span>";
				count++;
				i += index;
			}
		else if(rw(s, "char", i) == true){
				token += "<span class='reserved-word'> char </span>";
				count++;
				i += index;
			}
		else if(rw(s, "float", i) == true){
				token += "<span class='reserved-word'> float </span>";
				count++;
				i += index;
			}
		else if(rw(s, "string", i) == true){
				token += "<span class='reserved-word'> string </span>";
				count++;
				i += index;
			}
		else if(rw(s, "if", i) == true){
				token += "<span class='reserved-word'> if </span>";
				count++;
				i += index;
			}
		else if(rw(s, "else", i) == true){
				token += "<span class='reserved-word'> else </span>";
				count++;
				i += index;
			}
		else if(rw(s, "while", i) == true){
				token += "<span class='reserved-word'> while </span>";
				count++;
				i += index;
			}
		else if(rw(s, "for", i) == true){
				token += "<span class='reserved-word'> for </span>";
				count++;
				i += index;
			}
		else if(rw(s, "var", i) == true){
				token += "<span class='reserved-word'> var </span>";
				count++;
				i += index;
			}
		else if(rw(s, "let", i) == true){
				token += "<span class='reserved-word'> let </span>";
				count++;
				i += index;
			}
		else if(rw(s, "const", i) == true){
				token += "<span class='reserved-word'> const </span>";
				count++;
				i += index;
			}
		else if(rw(s, "include", i) == true){
				token += "<span class='reserved-word'> include </span>";
				count++;
				i += index;
			}
		else if(rw(s, "std", i) == true){
				token += "<span class='reserved-word'> std </span>";
				count++;
				i += index;
			}
		else if(rw(s, "return", i) == true){
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
		
		//Espacios
		else if(c == ' ') {
			token += " ";
		}
	}
}

//funcion que recibe el nombre de un archivo y lo lee si lo encuentra 
void DFA::lexerAritmetico(string archivo){
	string line;
	
	ifstream lee(archivo);
	if (lee.is_open()){
		while (getline(lee, line, '\n')){
			//procesar entrada
			//block
			proccesEntry(line);
			token += "<br>";
		}
		
		lee.close();
	}
	else{
		cout << "Unable to open file";
    }
}

//funcion para escribir el archivo .html
void DFA::write() {
	string head = "<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Actividad Integradora 3.4 Resaltador de sintaxis</title><link rel='stylesheet' href='css/style.css'></head><body>";
	
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

string DFA::toString() {
	return token;
}

int main(int argc, char* argv[]) {
	//tiempo inicial
	auto start = chrono::steady_clock::now();
	
	DFA dfa;
	
	dfa.lexerAritmetico("prueba1.txt");
	//cout << dfa.toString() << endl;
	dfa.write();
	
	//tiempo final
	auto end = chrono::steady_clock::now();
	
	//tiempo total
	double tiempo = double (chrono::duration_cast <chrono::nanoseconds> (end - start).count());
	cout << "Tiempo de ejecucion: " << tiempo/1e9 << "s" << endl;
}