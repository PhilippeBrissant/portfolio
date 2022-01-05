package DTO;

import java.util.Date;

import javax.validation.constraints.NotNull;
import javax.validation.constraints.Positive;

import Model.DescricaoTransacao;
import Model.FormaPagamento;
import Model.TransacaoModel;


public class PagamentoDTO {
    long id;

    @NotNull
    String cartao;
    @NotNull
    String descricao;
    @NotNull
    String valor;
    @Positive 
    int formaPagamento;
    @NotNull
    String parcelas;

    public PagamentoDTO(String cartao, String descricao, String valor, int formaPagamento, String parcelas) {
        this.cartao = cartao;
        this.descricao = descricao;
        this.valor = valor;
        this.formaPagamento = formaPagamento;
        this.parcelas = parcelas;
    }

    public TransacaoModel getPagamento() throws NumberFormatException {
        DescricaoTransacao descricaoTemp = new DescricaoTransacao(Double.valueOf(this.getValor()), new Date(), this.getDescricao());
        FormaPagamento.Tipo tipo = FormaPagamento.getTipo(this.getFormaPagamento());
        FormaPagamento formaPagamentoTemp = new FormaPagamento(tipo, Integer.valueOf(this.getParcelas()));

        return new TransacaoModel(this.cartao, descricaoTemp, formaPagamentoTemp);
    }

    
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getCartao() {
        return cartao;
    }

    public String getDescricao() {
        return descricao;
    }

    public String getValor() {
        return valor;
    }

    public int getFormaPagamento() {
        return formaPagamento;
    }

    public String getParcelas() {
        return parcelas;
    }

}
