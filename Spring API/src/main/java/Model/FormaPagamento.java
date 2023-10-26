package Model;

public class FormaPagamento {
    public enum Tipo {
        AVISTA,
        PARCELADO_LOJA,
        PARCELADO_EMISSOR,
        INVALIDO;
    }

    Tipo tipo;
    int parcelas;

    public FormaPagamento() {
        this.tipo = Tipo.AVISTA;
        this.parcelas = 1;
    }

    public FormaPagamento(Tipo formaPagamento, int parcelas) {
        this.tipo = formaPagamento;
        setParcelas(parcelas);
    }

    public Tipo getTipo() {
        return tipo;
    }

    public static Tipo getTipo(int tipo) {
        switch (tipo) {
            case 1:
                return Tipo.AVISTA;
            case 2:
                return Tipo.PARCELADO_LOJA;
            case 3:
                return Tipo.PARCELADO_EMISSOR;
            default:
                return Tipo.INVALIDO;

        }
    }

    public int getParcelas() {
        return parcelas;
    }

    private void setParcelas(int parcelas) {
        if (this.tipo == Tipo.AVISTA)
            this.parcelas = 1;
        else
            this.parcelas = parcelas;
    }

}
