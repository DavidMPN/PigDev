
class CPigAreaDeTexto: public CPigCaixaTexto{

private:

    int margemHorEsq,margemHorDir,margemVertCima,margemVertBaixo;
    int largMaxTexto;
    int espacoEntreLinhas;
    bool linhasAbaixoTexto;
    bool marcarMargem;

    PIG_Cor corLinhasTexto;

    void AjustaBaseTexto(int largParcial){

            while(xCursor>x+larg-margemHorDir){

                xBase-=5;
                xCursor = xBase + largParcial;

            }

            while(xCursor < x+margemHorEsq){

                xBase+=5;
                xCursor = xBase + largParcial;

            }

            while(yCursor < y + margemVertBaixo){

                yBase+=5;
                yCursor = yBase - ( espacoEntreLinhas*GetLinhaDoCursor());

            }

            while(yCursor > y +alt - margemVertCima){

                yBase-=5;
                yCursor = yBase - ( espacoEntreLinhas*GetLinhaDoCursor());

            }

    }

    void AjustaAlinhamento(){
    std::string textoBase(texto);
    std::string aux;

        xBase = x+margemHorEsq;
        yBase = y+alt-margemVertCima-GetTamanhoFonte(fonteTexto);

        aux.assign(textoBase,GetPosicaoInicioDaLinhaCursor(),posCursor - GetPosicaoInicioDaLinhaCursor());
        yCursor = yBase - ( espacoEntreLinhas*GetLinhaDoCursor());

        xCursor = xBase + CalculaLarguraPixels((char*)aux.c_str(),fonteTexto);

        AjustaBaseTexto(CalculaLarguraPixels((char*)aux.c_str(),fonteTexto));


    }

    void EscreveTexto(){

        EscreverLongaEsquerda(texto,xBase,yBase,largMaxTexto,espacoEntreLinhas,fonteTexto);

    }

    int TrataEventoMouse(PIG_Evento evento){

        TrataMouseRodinha(evento);
        TrataMouseClick(evento);

        return 0;
    }


/*****Exclusivo da classe*****/

    //Preciso no TrataEventoMouse, que vai ter que ficar como m�todo abstrato ja que outras caixas n�o usam isso
    int TrataMouseRodinha(PIG_Evento evento){

        std::string textoBase(texto);
        std::vector<std::string> linhas = ExtraiLinhasString(textoBase,largMaxTexto,fonteTexto);

        if(evento.mouse.acao == MOUSE_RODINHA){

            if(evento.mouse.relY == 1){

                if(yBaseOriginal < yBase){

                    yBase-=10;
                    yCursor-=10;

                }

            }

            if(evento.mouse.relY == -1){

                if( yBase < yBaseOriginal + ( espacoEntreLinhas*(linhas.size()-1))){

                    yBase+=10;
                    yCursor+=10;

                }

            }

            return 2;
        }

    }

    //Preciso no TrataEventoMouse
    int TrataMouseClick(PIG_Evento evento){

        if (evento.mouse.acao!=MOUSE_PRESSIONADO) return 0;
        std::string textoBase(texto);
        std::vector<std::string> linhas = ExtraiLinhasString(textoBase,largMaxTexto,fonteTexto);

        SDL_Point p;
        CMouse::PegaXY(p.x,p.y);
        SDL_Rect r = {x+margemHorEsq,y,larg-(margemHorEsq + margemHorDir),alt};

        if (SDL_PointInRect(&p,&r)){
            int delta = p.x-xBase;
            int largParcial = 0;
            int largUltimaLetra = 0;
            int inicioLinhaComMouseSobre = 0;

            inicioLinhaComMouseSobre = GetPosicaoInicioDaLinhaPosMouse();

            if(inicioLinhaComMouseSobre != -1){

            for (int i=inicioLinhaComMouseSobre;i<=textoBase.size();i++){
                std::string aux;

                aux.assign(textoBase,inicioLinhaComMouseSobre,i - inicioLinhaComMouseSobre);

                largParcial += CalculaLarguraPixels((char*)aux.c_str(),fonteTexto);

                if (delta<largParcial-largUltimaLetra){
                    posCursor = i-1;
                    AjustaAlinhamento();
                    return 1;
                }

                largUltimaLetra = largParcial;
            }

            }

            posCursor = strlen(texto);

            AjustaAlinhamento();
            if (estado==COMPONENTE_NORMAL)
                DefineEstado(COMPONENTE_EDITANDO);

            return 1;
        }


    }

    //Uso no AjustaAlinhamento para calcular a distancia do cursor
    int GetPosicaoInicioDaLinhaCursor(){

    int posPercorridas = 0;
    std::string textoBase(texto);
    std::vector<std::string> linhas = ExtraiLinhasString(textoBase,largMaxTexto,fonteTexto);


        for(int i=0;i<linhas.size();i++){

            if(GetLinhaDoCursor() == i){

                return posPercorridas;

            }

        posPercorridas += linhas[i].size();

        }

        return -1;

    }

    //Uso na GetPosicaoInicioDaLinhaPosMouse() para saber a linha que o mouse est� em cima
    int GetLinhaComMouseEmCima(){
    SDL_Point p;
    CMouse::PegaXY(p.x,p.y);
    std::string textoBase(texto);
    std::vector<std::string> linhas = ExtraiLinhasString(textoBase,largMaxTexto,fonteTexto);
    int yLinha=0;

        for(int i=0;i<linhas.size();i++){

            yLinha = yBase - (espacoEntreLinhas*i);
            if(p.y > yLinha && p.y <(yLinha + espacoEntreLinhas) ){

                return i;

            }

        }

        return (linhas.size()-1);

    }

    //Uso na TrataMouseClick para calcular o inicio da linha em que o mouse est�
    int GetPosicaoInicioDaLinhaPosMouse(){

    std::string textoBase(texto);
    //int linhaAtualCursor = GetLinhaDoCursor();
    int posPercorridas = 0;
    std::vector<std::string> linhas = ExtraiLinhasString(textoBase,largMaxTexto,fonteTexto);


        for(int i=0;i<linhas.size();i++){

            if(GetLinhaComMouseEmCima() == i){

                return posPercorridas;

            }

        posPercorridas += linhas[i].size();

        }

        return posPercorridas - linhas[linhas.size()-1].size();

    }

    //Uso na AjustaAlinhamento e AjustaBase, ela me ajuda a calcular o eixo y do cursor
    int GetLinhaDoCursor(){

    std::string textoBase(texto);
    std::vector<std::string> linhas = ExtraiLinhasString(textoBase,largMaxTexto,fonteTexto);
    int qntLinhas = 0;

    for(int i=0;i<linhas.size();i++){

        qntLinhas+=linhas[i].size();

        if(qntLinhas >= posCursor){

            return i;

        }

    }

    return 0;

    }

    //Uso na Desenha, marca as margens(ajuda na hora de alinhar o texto)
    void DesenhaMarcacaoMargem(){

        DesenhaLinhaSimples(x+margemHorEsq,y+margemVertBaixo,x+ margemHorEsq,y+alt-margemVertCima,BRANCO);
        DesenhaLinhaSimples(x+larg-margemHorDir-1,y+margemVertBaixo,x+larg-margemHorDir-1,y+alt-margemVertCima,BRANCO);

        DesenhaLinhaSimples(x+margemHorEsq,y+alt-margemVertCima,x+larg-margemHorDir,y+alt-margemVertCima,BRANCO);
        DesenhaLinhaSimples(x+margemHorEsq,y+margemVertBaixo,x+larg-margemHorDir,y+margemVertBaixo,BRANCO);


    }

    //Uso na desenha
    void DesenhaLinhasTexto(){

        int yLinha = yBase;
        int xLinha = x+margemHorEsq;
        int i=0;

        while(yLinha >= y + margemVertBaixo){

            DesenhaLinhaSimples(xLinha,yLinha,xLinha+larg-margemHorDir,yLinha,corLinhasTexto);
            i++;
            yLinha = yBase - (espacoEntreLinhas *i);

        }


    }

/**********************************/

public:

    CPigAreaDeTexto(int idComponente,int px, int py, int alt,int larg,char *nomeArq,int fonteDoTexto = 0,int fonteDoLabel = 0,int maxCars  = 200, bool apenasNumeros=false, int retiraFundo=1,int janela=0,int EspacoEntreLinhas =50,int LargMaxTexto =200,bool LinhasAbaixoTexto = false,bool marcarMargens = false):CPigCaixaTexto(idComponente,px,py,alt,larg,nomeArq,fonteDoTexto,fonteDoLabel,maxCars,apenasNumeros,retiraFundo,janela){

        espacoEntreLinhas = EspacoEntreLinhas;
        margemHorEsq = margemHorDir = margemVertCima = margemVertBaixo = 60;
        yBaseOriginal = y+alt-margemVertCima-GetTamanhoFonte(fonteTexto);
        xBaseOriginal = x+margemHorEsq;
        yBase = yBaseOriginal;
        xBase = xBaseOriginal;
        xCursor = xBase;
        yCursor = yBase;
        largMaxTexto = LargMaxTexto;
        linhasAbaixoTexto = LinhasAbaixoTexto;
        corLinhasTexto = PRETO;
        marcarMargem = marcarMargens;

    }

    ~CPigAreaDeTexto(){
        if (timer) delete timer;
    }

    int Desenha(){
        //imagem de fundo
        SDL_RenderCopyEx(renderer, text, &frame,&dest,-angulo,&pivoRelativo,flip);

        //bloqueia a area fora do componente
        //SDL_Rect r={x+margemHor-2,altJanela-y-alt,larg-2*(margemHor-2),alt};
        SDL_Rect r={x+margemHorEsq,altJanela-y-alt+margemVertCima,larg-(margemHorEsq+margemHorDir),alt-(margemVertBaixo+margemVertCima)+1};
        SDL_RenderSetClipRect(renderer,&r);

        EscreveTexto();
        DesenhaCursor();
        if(linhasAbaixoTexto) DesenhaLinhasTexto();
        if(marcarMargem) DesenhaMarcacaoMargem();


        //desbloqueia o desenho fora da area do componente
        SDL_RenderSetClipRect(renderer,NULL);

        DesenhaLabel();

    }

    void SetMargens(int horEsq,int horDir, int vertBaixo,int vertCima){
        margemVertCima = vertCima;
        margemVertBaixo = vertBaixo;
        margemHorDir = horDir;
        margemHorEsq = horEsq;
        yBaseOriginal = y+alt-margemVertCima-GetTamanhoFonte(fonteTexto);
        xBaseOriginal = x+margemHorEsq;
        AjustaAlinhamento();
    }

    /*****Exclusivo da classe*****/

    //Muda a cor das Linhas abaixo do texto
    void SetCorLinhasTexto(PIG_Cor cor){

        corLinhasTexto = cor;

    }

    //Seta a Largura max do texto
    void SetLargMaxTexto(int largMax){

        largMaxTexto = largMax;

    }

    void SetEspacoEntreAsLinhas(int espaco){

        espacoEntreLinhas = espaco + GetTamanhoFonte(fonteTexto);

    }

    /**********************************/

};

typedef CPigAreaDeTexto *PigAreaDeTexto;
