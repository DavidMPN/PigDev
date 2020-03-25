class CPigCaixaTexto: public CPigComponente{

protected:

    char texto[300],label[300];
    PIG_PosicaoComponente posLabel;
    int labelX,labelY;
    int fonteTexto,fonteLabel;
    int posCursor;
    int xBase,xCursor,yCursor,yBase;
    bool cursorExibido;
    int maxCaracteres;
    bool somenteNumeros;
    bool marcarMargem;

    PIG_Cor corCursor;
    Timer timer;

    virtual void AjustaAlinhamento();

    int TrataEventoTeclado(PIG_Evento evento){

        if (evento.teclado.acao==TECLA_EDICAO){
            return 1;
        }else if (evento.teclado.acao==TECLA_INPUT){
            if (AdicionaTexto((char*)ConverteString(evento.teclado.texto).c_str())){
                if (audio>=0) PlayAudio(audio);
                return 1;
            }else return 0;
        }else if (evento.teclado.acao==TECLA_PRESSIONADA){
            switch (evento.teclado.tecla){
            case TECLA_BACKSPACE:
                RetiraTextoBackSpace();break;
            case TECLA_DELETE:
                RetiraTextoDelete();break;
            case TECLA_DIREITA:
                AvancaCursor();break;
            case TECLA_ESQUERDA:
                VoltaCursor();break;
            }
            return 1;
        }
        return 0;

    }

    int AdicionaTexto(char *frase){
        if (strlen(texto)+strlen(frase)>maxCaracteres) return 0;
        if (somenteNumeros&&!SomenteNumeros(frase)) return 0;

        char fim[300]="";
        strcpy(fim,&texto[posCursor]);
        texto[posCursor]='\0';
        strcat(texto,frase);
        strcat(texto,fim);

        AvancaCursor();
        return 1;
    }

private:

    int OnMouseOn(){}
    int OnMouseOff(){}

    virtual void AjustaBaseTexto(int largParcial);
    virtual int TrataEventoMouse(PIG_Evento evento);
    virtual void EscreveTexto();

    int AvancaCursor(){
        if (posCursor>=strlen(texto)) return 0;
        posCursor++;
        AjustaAlinhamento();
        return 1;
    }

    int VoltaCursor(){
        if (posCursor==0) return 0;
        posCursor--;
        AjustaAlinhamento();
        return 1;
    }

    int RetiraTextoBackSpace(){
        if (strlen(texto)==0||posCursor==0) return 0;

        for (int i=posCursor-1;i<strlen(texto);i++)
            texto[i]=texto[i+1];

        VoltaCursor();
        if (audio>=0) PlayAudio(audio);
        return 1;
    }

    int RetiraTextoDelete(){
        if (strlen(texto)==0||posCursor>=strlen(texto)) return 0;

        for (int i=posCursor;i<strlen(texto);i++)
            texto[i]=texto[i+1];

        if (audio>=0) PlayAudio(audio);
        return 1;
    }

    std::string ConverteString(char *str){
        std::string resp(str);
        if (strlen(str)>1){
            if ((uint8_t)str[0]==195){
                resp = str[1]+64;
            }else if ((uint8_t)str[0]==194){
                resp = str[1];
            }

        }
        return resp;
    }

public:

    virtual int TrataEvento(PIG_Evento evento);
    virtual int Desenha();

    CPigCaixaTexto(int idComponente,int px, int py, int alt,int larg,char *nomeArq,int fonteDoTexto = 0,int fonteDoLabel = 0,int maxCars = 200,bool apenasNumeros=false,int retiraFundo=1,int janela=0):CPigComponente(idComponente,px,py,alt,larg,nomeArq,retiraFundo,janela){

        posLabel = COMPONENTE_ESQUERDA;//posição padrão do label
        fonteLabel = fonteDoLabel;
        fonteTexto = fonteDoTexto;//fonte padrão
        posCursor = 0;//cursor no início do texto
        cursorExibido = true;
        timer = NULL;//o timer só será criado quando estiver editando
        estado = COMPONENTE_NORMAL;
        maxCaracteres = maxCars;
        strcpy(texto,"");
        strcpy(label,"");
        somenteNumeros = apenasNumeros;
        corCursor = PRETO;

    }

    ~CPigCaixaTexto(){
        if (timer) delete timer;
    }

    int SetTexto(char *frase){
        strcpy(texto,"");
        posCursor=0;
        AdicionaTexto(frase);
        return 1;
    }

    void SetFonteLabel(int idFonte){

        fonteLabel = idFonte;

    }

    int SetLabel(char *frase){
        strcpy(label,frase);
        return 1;
    }

    void GetTexto(char *buffer){
        strcpy(buffer,texto);
    }

    void SetCorCursor(PIG_Cor cor){
        corCursor = cor;
    }

    void SetPosicaoPadraoLabel(PIG_PosicaoComponente pos){
        posLabel = pos;
    }

    void SetPosicaoPersonalizadaLabel(int rx, int ry){
        labelX = rx;
        labelY = ry;
    }

    void DefineEstado(PIG_EstadoComponente estadoComponente){

        estado = estadoComponente;
        if (estado==COMPONENTE_EDITANDO){
            timer = new CTimer(false);
            SDL_StartTextInput();
        }else if (estado==COMPONENTE_NORMAL){
            SDL_StopTextInput();
            if (timer) delete timer;
        }

    }

    void Move(int nx, int ny){
        CPigComponente::Move(nx,ny);
        AjustaAlinhamento();
    }


};

typedef CPigCaixaTexto *PigCaixaTexto;
