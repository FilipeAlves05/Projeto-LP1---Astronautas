#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Astronauta {
public:
    string cpf;
    string nome;
    int idade;
    bool disponivel;
    bool vivo;
    vector<int> voosParticipados;

    Astronauta() : cpf(""), nome(""), idade(0), disponivel(true), vivo(true) {}

    Astronauta(string cpf, string nome, int idade)
        : cpf(cpf), nome(nome), idade(idade), disponivel(true), vivo(true) {}

    void imprimir() const {
        cout << "CPF: " << cpf << ", Nome: " << nome << ", Idade: " << idade
             << ", Disponível: " << (disponivel ? "Sim" : "Não")
             << ", Vivo: " << (vivo ? "Sim" : "Não") << endl;
    }
};

class Voo {
public:
    int codigo;
    enum Status { EM_PLANEJAMENTO, EM_CURSO, FINALIZADO } status;
    vector<Astronauta*> passageiros;
    bool finalizadoComSucesso;

    Voo() : codigo(0), status(EM_PLANEJAMENTO), finalizadoComSucesso(false) {}

    Voo(int codigo) : codigo(codigo), status(EM_PLANEJAMENTO), finalizadoComSucesso(false) {}

    void adicionarPassageiro(Astronauta* astronauta) {
        if (status == EM_PLANEJAMENTO) {
            passageiros.push_back(astronauta);
            astronauta->voosParticipados.push_back(codigo);
        } else {
            cout << "Não é possível adicionar astronautas a um voo que não está em planejamento." << endl;
        }
    }

    void removerPassageiro(const string& cpf) {
        if (status == EM_PLANEJAMENTO) {
            for (auto it = passageiros.begin(); it != passageiros.end(); ++it) {
                if ((*it)->cpf == cpf) {
                    passageiros.erase(it);
                    return;
                }
            }
            cout << "Astronauta com CPF " << cpf << " não encontrado no voo." << endl;
        } else {
            cout << "Não é possível remover astronautas de um voo que não está em planejamento." << endl;
        }
    }

    void lancar() {
        if (status == EM_PLANEJAMENTO && !passageiros.empty()) {
            for (const auto& astronauta : passageiros) {
                if (!astronauta->disponivel) {
                    cout << "Todos os astronautas devem estar disponíveis para lançar o voo." << endl;
                    return;
                }
            }
            status = EM_CURSO;
            for (auto& astronauta : passageiros) {
                astronauta->disponivel = false;
            }
            cout << "Voo " << codigo << " lançado com sucesso!" << endl;
        } else {
            cout << "O voo deve ter pelo menos um astronauta para ser lançado." << endl;
        }
    }

    void explodir() {
        if (status == EM_CURSO) {
            for (auto& astronauta : passageiros) {
                astronauta->vivo = false;
            }
            status = FINALIZADO;
            finalizadoComSucesso = false;
            cout << "Voo " << codigo << " explodiu. Todos os astronautas morreram." << endl;
        } else {
            cout << "Somente voos em curso podem explodir." << endl;
        }
    }

    void finalizarComSucesso() {
        if (status == EM_CURSO) {
            status = FINALIZADO;
            finalizadoComSucesso = true;
            for (auto& astronauta : passageiros) {
                astronauta->disponivel = true;
            }
            cout << "Voo " << codigo << " finalizado com sucesso! Todos os astronautas estão disponíveis novamente." << endl;
        } else {
            cout << "Somente voos em curso podem ser finalizados com sucesso." << endl;
        }
    }

    void imprimir() const {
        cout << "Código do Voo: " << codigo << ", Status: ";
        switch (status) {
            case EM_PLANEJAMENTO: cout << "Em Planejamento"; break;
            case EM_CURSO: cout << "Em Curso"; break;
            case FINALIZADO: 
                cout << "Finalizado"; 
                if(finalizadoComSucesso) {
                    cout << " (com sucesso)";
                } else {
                    cout << " (explodiu)";
                }
                break;
        }
        cout << ", Passageiros: " << passageiros.size() << endl;
        for (const auto& astronauta : passageiros) {
            astronauta->imprimir();
        }
    }
};

class Sistema {
public:
    unordered_map<string, Astronauta> astronautas;
    unordered_map<int, Voo> voos;

    void cadastrarAstronauta(const Astronauta& astronauta) {
        astronautas[astronauta.cpf] = astronauta;
    }

    void cadastrarVoo(const Voo& voo) {
        voos[voo.codigo] = voo;
    }

    void adicionarAstronautaAoVoo(const string& cpf, int codigoVoo) {
        if (astronautas.find(cpf) != astronautas.end() && voos.find(codigoVoo) != voos.end()) {
            voos[codigoVoo].adicionarPassageiro(&astronautas[cpf]);
        } else {
            cout << "Astronauta ou Voo não encontrado." << endl;
        }
    }

    void removerAstronautaDoVoo(const string& cpf, int codigoVoo) {
        if (voos.find(codigoVoo) != voos.end()) {
            voos[codigoVoo].removerPassageiro(cpf);
        } else {
            cout << "Voo não encontrado." << endl;
        }
    }

    void lancarVoo(int codigoVoo) {
        if (voos.find(codigoVoo) != voos.end()) {
            voos[codigoVoo].lancar();
        } else {
            cout << "Voo não encontrado." << endl;
        }
    }

    void explodirVoo(int codigoVoo) {
        if (voos.find(codigoVoo) != voos.end()) {
            voos[codigoVoo].explodir();
        } else {
            cout << "Voo não encontrado." << endl;
        }
    }

    void finalizarVooComSucesso(int codigoVoo) {
        if (voos.find(codigoVoo) != voos.end()) {
            voos[codigoVoo].finalizarComSucesso();
        } else {
            cout << "Voo não encontrado." << endl;
        }
    }

    void listarVoos() const {
        cout << "Voos Planejados:" << endl;
        for (const auto& pair : voos) {
            if (pair.second.status == Voo::EM_PLANEJAMENTO) {
                pair.second.imprimir();
            }
        }
        cout << "\nVoos em Curso:" << endl;
        for (const auto& pair : voos) {
            if (pair.second.status == Voo::EM_CURSO) {
                pair.second.imprimir();
            }
        }
        cout << "\nVoos Finalizados:" << endl;
        for (const auto& pair : voos) {
            if (pair.second.status == Voo::FINALIZADO) {
                pair.second.imprimir();
            }
        }
    }

    void listarAstronautasMortos() const {
        cout << "\nAstronautas Mortos:" << endl;
        for (const auto& pair : astronautas) {
            const Astronauta& astronauta = pair.second;
            if (!astronauta.vivo) {
                cout << "CPF: " << astronauta.cpf << ", Nome: " << astronauta.nome << ", Voos Participados: ";
                for (const int& voo : astronauta.voosParticipados) {
                    cout << voo << " ";
                }
                cout << endl;
            }
        }
    }
};

void exibirMenu() {
    cout << "\nSistema de Gestão de Voos Espaciais\n";
    cout << "1. Cadastrar Astronauta\n";
    cout << "2. Cadastrar Voo\n";
    cout << "3. Adicionar Astronauta ao Voo\n";
    cout << "4. Remover Astronauta do Voo\n";
    cout << "5. Lançar Voo\n";
    cout << "6. Explodir Voo\n";
    cout << "7. Finalizar Voo com Sucesso\n";
    cout << "8. Listar Voos\n";
    cout << "9. Listar Astronautas Mortos\n";
    cout << "0. Sair\n";
    cout << "Escolha uma opção: ";
}

int main() {
    Sistema sistema;
    int opcao;

    do {
        exibirMenu();
        cin >> opcao;

        switch (opcao) {
            case 1: {
                string cpf, nome;
                int idade;
                cout << "CPF: ";
                cin >> cpf;
                cout << "Nome: ";
                cin.ignore();
                getline(cin, nome);
                cout << "Idade: ";
                cin >> idade;
                sistema.cadastrarAstronauta(Astronauta(cpf, nome, idade));
                break;
            }
            case 2: {
                int codigo;
                cout << "Código do Voo: ";
                cin >> codigo;
                sistema.cadastrarVoo(Voo(codigo));
                break;
            }
            case 3: {
                string cpf;
                int codigoVoo;
                cout << "CPF do Astronauta: ";
                cin >> cpf;
                cout << "Código do Voo: ";
                cin >> codigoVoo;
                sistema.adicionarAstronautaAoVoo(cpf, codigoVoo);
                break;
            }
            case 4: {
                string cpf;
                int codigoVoo;
                cout << "CPF do Astronauta: ";
                cin >> cpf;
                cout << "Código do Voo: ";
                cin >> codigoVoo;
                sistema.removerAstronautaDoVoo(cpf, codigoVoo);
                break;
            }
            case 5: {
                int codigoVoo;
                cout << "Código do Voo: ";
                cin >> codigoVoo;
                sistema.lancarVoo(codigoVoo);
                break;
            }
            case 6: {
                int codigoVoo;
                cout << "Código do Voo: ";
                cin >> codigoVoo;
                sistema.explodirVoo(codigoVoo);
                break;
            }
            case 7: {
                int codigoVoo;
                cout << "Código do Voo: ";
                cin >> codigoVoo;
                sistema.finalizarVooComSucesso(codigoVoo);
                break;
            }
            case 8: {
                sistema.listarVoos();
                break;
            }
            case 9: {
                sistema.listarAstronautasMortos();
                break;
            }
            case 0: {
                cout << "Saindo do sistema..." << endl;
                break;
            }
            default: {
                cout << "Opção inválida. Tente novamente." << endl;
            }
        }
    } while (opcao != 0);

    return 0;
}