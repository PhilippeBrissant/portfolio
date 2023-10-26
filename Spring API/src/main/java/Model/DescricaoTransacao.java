package Model;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import enums.StatusTransacao;

public class DescricaoTransacao {
    private double valor;
    private Date data;
    private String estabelecimento;
    private String nsu;
    private String codigoAutorizacao;
    private StatusTransacao status;

    public DescricaoTransacao(double valor, Date data, String estabelecimento) {
        this.valor = valor;
        this.data = data;
        this.estabelecimento = estabelecimento;
    }

    public String getValor() {
        return new DecimalFormat("0.00").format(valor);
    }

    public String getData() {
        return new SimpleDateFormat("dd/MM/yyyy H:mm:ss").format(data);
    }

    public String getEstabelecimento() {
        return estabelecimento;
    }

    public String getNsu() {
        return nsu;
    }

    public DescricaoTransacao setNsu(String nsu) {
        this.nsu = nsu;
        return this;
    }

    public String getCodigoAutorizacao() {
        return codigoAutorizacao;
    }

    public DescricaoTransacao setCodigoAutorizacao(String codigoAutorizacao) {
        this.codigoAutorizacao = codigoAutorizacao;
        return this;
    }

    public StatusTransacao getStatus() {
        return status;
    }

    public DescricaoTransacao setStatus(StatusTransacao status) {
        this.status = status;
        return this;
    }
}
