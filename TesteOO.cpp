#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
//Em C algumas funcoes eram do tipo int e eu retornarva 1 ou 0.Em C++, é uma boa pratica usar bool como tipo das funcoes


// Definindo a classe Pilha como template para suportar tipos genericos
template <typename T> // template para classe imediatamente depois
class Pilha{
private:
    static const int TAM_MAX = 1000;
    T valores[TAM_MAX];
    int topo;

public:
    //construtor
    Pilha(){
        topo = -1; //o compilador associa ao membro da classe pois nao ha ambiguidades de parametro e topo fora declarado como private
    }

    bool is_full(){
        if(topo == TAM_MAX - 1)
            return true;
        else
            return false;
    }

    bool is_empty(){
        if(topo == -1)
            return true;
        else
            return false;
    }

    T top(){
        if(!is_empty())
            return valores[topo];
        throw std::runtime_error("Erro: Pilha vazia.");
    }

    int tamanho(){
        return topo+1;
    }

    bool push(T x){
        if(!is_full()){
            valores[topo+1] = x;
            topo++;
            return true;
        }
        else
            return false;
    }

    bool x_push_vetor(T v[], int n){
        if((topo + n) < TAM_MAX){
            for(int i=0; i<n; i++){
                valores[topo+1] = v[i];
                topo++;
            }
            return true;
        }
        else
            return false;
    }

    bool x_push_pilha(Pilha<T> aux){
        int num_elementos = aux.tamanho();

        if(tamanho() + num_elementos <= TAM_MAX){
            for(int i=0; i<aux.tamanho(); i++){
                valores[topo+1] = aux.valores[i];
                topo++;
            }
            return true;
        }
        return false;
    }

    void Imprimi(){
        if(!is_empty()){
            for(int i = topo; i >= 0; i--)
                std::cout << "| " << valores[i] << " |\n";
        }
        else
            std::cout << "Pilha vazia\n";

    }

    T pop(){
        if(!is_empty()){
            topo--;
            return valores[topo+1];
        }
        else
            return T();
    }

    void pop_n(int n){
        if(n <= tamanho()){
            for(int i=0; i<n; i++)
                pop();
        }
        else{
            while(!is_empty())
                pop();
        }
    }

    Pilha<T> clonar()const{
        Pilha<T> clone;

        //clone = original
        for(int i = 0; i <= topo; i++)
            clone.push(valores[i]);

        return clone;
    }

    void clear(){
        topo = -1; // Resetando o topo, os valores antigos sao sobrescritos
    }

    void inverter() {
        if(!is_empty()){
            Pilha<T> clone = clonar(); // Cria um clone da pilha atual
            clear(); // limpa a pilha original

            //adiciona na pilha (invertido) maior indice para o menor.
            for(int i = clone.tamanho() - 1; i >= 0; i--)
                push(clone.valores[i]);
        }
    }

    int buscar(const T& elemento){
        for(int i = 0; i <= topo; i++)
            if(valores[i] == elemento)
                return i; // Retorna posicao

        return -1; //nao encontrou
    }

};

class Principal{ //a main principal nos tras para ca.
public:
    static void main(){ //Logo, esse e o metodo que esta sendo usado como "logica principal de execucao"
        int opcao;      //MAS, o codigo de entrada(main do final do programa) continua existindo por ser obrigatorio em C++.
        std::cout << "Escolha o tipo de dado da pilha:\n";
        std::cout << "1) int\n2) float\n3) char\n";
        std::cout << "Digite sua escolha: ";
        std::cin >> opcao;

        if(opcao == 1){
            Pilha<int> p;
            menu(p);
        }
        else if(opcao == 2){
            Pilha<float> p;
            menu(p);
        }
        else if(opcao == 3){
            Pilha<char> p;
            menu(p);
        }
        else{
            std::cout << "Opcao invalida.\n";
        }
    }

    template <typename T>
    static void submenu_adicao(Pilha<T>& p){
        int opcao = 0;
        while(opcao != 4){
            std::cout << "\nSubmenu de Adicao:\n";
            std::cout << "1. Empilhar elemento (push)\n";
            std::cout << "2. Adicionar vetor de elementos\n";
            std::cout << "3. Adicionar outra pilha\n";
            std::cout << "4. Voltar ao menu principal\n";
            std::cout << "\nEscolha uma opcao: ";
            std::cin >> opcao;

            switch(opcao){
                case 1:{
                    T valor;
                    std::cout << "\nDigite o valor a ser empilhado: ";
                    std::cin >> valor;
                    if(p.push(valor))
                        std::cout << "Elemento empilhado com sucesso!\n";
                    else
                        std::cout << "Erro: Pilha cheia.\n";
                    break;
                }
                case 2:{
                    int n;
                    std::cout << "\nDigite o numero de elementos do vetor: ";
                    std::cin >> n;
                    if(n > 0){
                        T* vetor = new T[n]; //aloco dinamicamente(vetor)
                        std::cout << "Digite os elementos do vetor: ";
                        for(int i = 0; i < n; i++)
                            std::cin >> vetor[i];

                        if(p.x_push_vetor(vetor, n))
                            std::cout << "Vetor empilhado com sucesso!\n";
                        else
                            std::cout << "Erro: Pilha cheia.\n";

                        delete[] vetor; //desaloco (vetor)
                    }
                    else
                        std::cout << "Erro: Numero de elementos invalido.\n";
                    break;
                }
                case 3:{
                    Pilha<T> outra_pilha;
                    if constexpr(std::is_same<T, int>::value){
                        int valores[] = {1, 2, 3, 4, 5};
                        outra_pilha.x_push_vetor(valores, 5);
                    }
                    else if constexpr(std::is_same<T, float>::value){
                        float valores[] = {1.1, 2.2, 3.3, 4.4, 5.5};
                        outra_pilha.x_push_vetor(valores, 5);
                    }
                    else if constexpr(std::is_same<T, char>::value){
                        char valores[] = {'A', 'B', 'C', 'D', 'E'};
                        outra_pilha.x_push_vetor(valores, 5);
                    }
                    else{
                        std::cout << "Erro: Tipo nao suportado.\n";
                        break;
                    }

                    if(p.x_push_pilha(outra_pilha))
                        std::cout << "Pilha adicionada com sucesso!\n";
                    else
                        std::cout << "Erro: Pilha cheia.\n";
                    break;
                }
                case 4:
                    std::cout << "Voltando ao menu principal.\n";
                    break;
                default:
                    std::cout << "Opcao invalida. Tente novamente.\n";
                    break;
            }
        }
    }


    template <typename T>
    static void submenu_remocao(Pilha<T>& p){
        int opcao = 0;
        while(opcao != 3){
            std::cout << "\nSubmenu de Remocao:\n";
            std::cout << "1. Desempilhar elemento (pop)\n";
            std::cout << "2. Remover varios elementos (pop_n)\n";
            std::cout << "3. Voltar ao menu principal\n";
            std::cout << "\nEscolha uma opcao: ";
            std::cin >> opcao;

            switch(opcao){
                case 1:{
                    if(!p.is_empty()){
                        T valor = p.pop();
                        std::cout << "Elemento " << valor << " desempilhado.\n";
                    }
                    else
                        std::cout << "Erro: Pilha vazia.\n";

                    break;
                }
                case 2:{
                    int n;
                    std::cout << "Digite o numero de elementos a remover: ";
                    std::cin >> n;
                    p.pop_n(n);
                    std::cout << "Elementos removidos.\n";
                    break;
                }
                case 3:
                    std::cout << "Voltando ao menu principal.\n";
                    break;
                default:
                    std::cout << "Opcao invalida. Tente novamente.\n";
                    break;
            }
        }
    }



    template <typename T>
    static void menu(Pilha<T>& p){
        int opcao = 0;
        while(opcao != 10){
            std::cout << "\nMenu Principal:\n";
            std::cout << "1. Acessar o Submenu de Adicao\n";
            std::cout << "2. Acessar o Submenu de Remocao\n";
            std::cout << "3. Imprimir pilha\n";
            std::cout << "4. Verificar tamanho da pilha\n";
            std::cout << "5. Verificar se a pilha esta vazia\n";
            std::cout << "6. Inverter a pilha\n";
            std::cout << "7. Buscar um elemento na pilha\n";
            std::cout << "8. Clonar a pilha\n";
            std::cout << "9. Topo da pilha\n";
            std::cout << "10. Sair\n";
            std::cout << "\nEscolha uma opcao: ";
            std::cin >> opcao;

            switch(opcao){
                case 1:
                    submenu_adicao(p);
                    break;
                case 2:
                    submenu_remocao(p);
                    break;
                case 3:
                    std::cout << "\nConteudo da pilha: \n";
                    p.Imprimi();
                    break;
                case 4:
                    std::cout << "Tamanho da pilha: " << p.tamanho() << "\n";
                    break;
                case 5:
                    if (p.is_empty())
                        std::cout << "A pilha esta vazia.\n";
                    else
                        std::cout << "A pilha nao esta vazia.\n";
                    break;
                case 6:
                    p.inverter();
                    std::cout << "Pilha invertida com sucesso!\n";
                    break;
                case 7:{
                    T valor;
                    std::cout << "Digite o elemento a ser buscado: ";
                    std::cin >> valor;
                    int posicao = p.buscar(valor);
                    if(posicao != -1)
                        std::cout << "Elemento encontrado na posicao " << posicao << ".\n";
                    else
                        std::cout << "Elemento nao encontrado na pilha.\n";
                    break;
                }
                case 8:{
                    Pilha<T> clone = p.clonar();
                    std::cout << "Pilha clonada com sucesso! Conteudo da pilha clonada:\n";
                    clone.Imprimi();
                    break;
                }
                case 9:{
                    if(!p.is_empty())
                        std::cout << "Elemento no topo da pilha: " << p.top() << "\n";
                    else
                        std::cout << "Erro: Pilha vazia.\n";
                    break;
                }
                case 10:
                    std::cout << "Encerrando o programa.\n";
                    break;
                default:
                    std::cout << "Opcao invalida. Tente novamente.\n";
                    break;
            }
        }
    }

};

int main(){
    Principal::main(); //o fluxo e transferido para um metodo da classe Principal( nome do metodo: main (poderia ser outro nome) )
}
