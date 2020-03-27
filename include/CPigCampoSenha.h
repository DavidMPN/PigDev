
class CPigCampoSenha: public CPigCaixaTexto{

private:

    int margemHor,margemVert;
    char mascara;

    void AjustaBaseTexto(int largParcial){

            while(xCursor>x+larg-margemHor){

                xBase-=5;
                xCursor = xBase + largParcial;

            }

            while(xCursor < x+margemHor){

                xBase+=5;
                xCursor = xBase + largParcial;

            }

    }

    void AjustaAlinhamento(){
    std::string textoBase = GetTextoMask();
    std::string aux;

        xBase = x+margemHor;

        aux.assign(textoBase,0,posCursor);
        xCursor = xBase + CalculaLarguraPixels((char*)aux.c_str(),fonteTexto);

        AjustaBaseTexto(CalculaLarguraPixels((char*)aux.c_str(),fonteTexto));


    }

    void EscreveTexto(){

        EscreverEsquerda((char*)GetTextoMask().c_str(),xBase,yBase,fonteTexto);

    }

    int TrataEventoMouse(PIG_Evento evento){

        SDL_Point p;
        CMouse::PegaXY(p.x,p.y);
        SDL_Rect r = {x+margemHor,y,larg-(2 * margemHor),alt};

        if (SDL_PointInRect(&p,&r)){

            if (evento.mouse.acao == MOUSE_PRESSIONADO && evento.mouse.botao == MOUSE_ESQUERDO) return TrataMouseBotaoEsquerdo(evento,p);

        }

        return 0;
    }

    int TrataMouseBotaoEsquerdo(PIG_Evento evento,SDL_Point p){
        int delta = p.x-xBase;
        int largParcial = 0;
        int largUltimaLetra = 0;
        std::string textoBase(GetTextoMask());

        for (int i=0;i<=textoBase.size();i++){
            std::string aux;

            aux.assign(textoBase,0,i);

            largParcial += CalculaLarguraPixels((char*)aux.c_str(),fonteTexto);

            if (delta<largParcial-largUltimaLetra){
                posCursor = i-1;
                AjustaAlinhamento();
                return 1;
            }

            largUltimaLetra = largParcial;
        }

        posCursor = strlen(texto);

        AjustaAlinhamento();
        if (estado==COMPONENTE_NORMAL)
            DefineEstado(COMPONENTE_EDITANDO);

        return 1;


    }

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

/*****Exclusivo da classe*****/

    std::string GetTextoMask(){
        std::string resp;
        for (int i=0;i<strlen(texto);i++)
            resp += mascara;

        return resp;
    }

/**********************************/

public:

    CPigCampoSenha(int idComponente,int px, int py, int alt,int larg,char *nomeArq,int fonteDoTexto = 0,int fonteDoLabel = 0,int maxCars = 200, bool apenasNumeros=false, int retiraFundo=1,int janela=0):CPigCaixaTexto(idComponente,px,py,alt,larg,nomeArq,fonteDoTexto,fonteDoLabel,maxCars,apenasNumeros,retiraFundo,janela){

            yBase = y+margemVert;
            xBase = x+margemHor;
            xCursor = xBase;
            yCursor = yBase;
            margemHor= margemVert = 0;
            mascara = '*';

    }

    ~CPigCampoSenha(){
        if (timer) delete timer;
    }

    int Desenha(){
        //imagem de fundo
        SDL_RenderCopyEx(renderer, text, &frame,&dest,-angulo,&pivoRelativo,flip);

        //SDL_Rect r={x+margemHor-2,altJanela-y-alt,larg-2*(margemHor-2),alt};
        SDL_Rect r={x+margemHor,altJanela-y-alt+margemVert,larg-(2*margemHor),alt-(2*margemVert)+1};
        SDL_RenderSetClipRect(renderer,&r);

        EscreveTexto();
        DesenhaCursor();

        //desbloqueia o desenho fora da area do componente
        SDL_RenderSetClipRect(renderer,NULL);

        DesenhaLabel();

    }

    void SetMargens(int margHor,int margVert){

        margemHor = margHor;
        margemVert = margVert;
        yBaseOriginal = y+margemVert;
        xBaseOriginal = x+margemHor;
        yBase = yBaseOriginal;
        xBase = xBaseOriginal;
        xCursor = xBase;
        yCursor = yBase;
        AjustaAlinhamento();

    }

};

typedef CPigCampoSenha *PigCampoSenha;
