/*
####################################
# Actividad 3.2 Programando un DFA #
#                                  #
# Daniel Cruz Arciniega            #
# A01701370                        #
####################################
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <fstream>
#include <vector>

using namespace std;

class DFA {
	private:
		vector<string> token; 
		vector<string> tipo;
		int count; //Variable para saber cuantos elementos hay en los vectores
 		int index; //variable auxiliar en caso de que se requieran saltar índices en el ciclo for de lexerAritmetico
		
	public:
		DFA() {count = 0; index=0;}
		void proccesEntry(string);
		void lexerAritmetico(string);
		string var(string, int);
		bool isFloat(string, int);
		string nfloat(string, int);
		string entero(string, int);
		string toString();
};

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
		
		if(c == '/' && s[i+1] == '/') {
			for(int j = i; j < s.size(); j++)
				aux << s[j];
			token.push_back(aux.str());
			tipo.push_back("Comentario");
			count++;
			break;
				
		}
		
		else if(isalpha(c)) {
			index = 0;
			aux << c;
			aux << var(s, i+1);
			token.push_back(aux.str());
			tipo.push_back("Variable");
			count++;
			i += index;
		}
		
		else if(isdigit(c) || (c == '-' && isdigit(s[i+1]) && s[i-1] == ('('))) {
			index = 0;
			aux << c;
			
			switch(isFloat(s, i+1)){
				case true:
					aux << nfloat(s, i+1);
					token.push_back(aux.str());
					tipo.push_back("Real");
					count++;
					i += index;
					break;
				case false:
					aux << nfloat(s, i+1);
					token.push_back(aux.str());
					tipo.push_back("Entero");
					count++;
					i += index;
					break;
			}
		}
		
		else if(c == '=') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("Asignación");
			count++;
		}
		
		else if(c == '+') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("Suma");
			count++;
		}
		
		else if(c == '-') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("Resta");
			count++;
		}
		
		else if(c == '*') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("Multiplicación");
			count++;
		}
		
		else if(c == '/') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("División");
			count++;
		}
		
		else if(c == '^') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("Potencia");
			count++;
		}
		
		else if(c == '(') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("Paréntesis que abre");
			count++;
		}
		
		else if(c == ')') {
			aux << c;
			token.push_back(aux.str());
			tipo.push_back("Paréntesis que cierra");
			count++;
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
			proccesEntry(line);
		}
		
		lee.close();
	}
	else{
		cout << "Unable to open file";
    }
}

string DFA::toString() {
	stringstream aux;
	
	aux << "Token    Tipo\n"; 
	for (unsigned int i = 0; i < count; i++) {
		aux << token[i] << " | " << tipo[i] << "\n";
	}
	return aux.str();
}

int main(int argc, char* argv[]) {
	DFA dfa;
	string input;
	
	cout << "input: ";
	getline(cin, input);
	
	dfa.lexerAritmetico(input);
	cout << dfa.toString() << endl;
}