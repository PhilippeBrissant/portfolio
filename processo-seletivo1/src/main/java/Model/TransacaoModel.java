package Model;

public class TransacaoModel {
    private long id;
    private String cartao;
    private DescricaoTransacao descricao;
    private FormaPagamento formaPagamento;

    public TransacaoModel() {
    }

    public TransacaoModel(String cartao, DescricaoTransacao descricaoTransacao, FormaPagamento formaPagamento) {
        this.cartao = cartao;
        this.descricao = descricaoTransacao;
        this.formaPagamento = formaPagamento;
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

    public void setCartao(String cartao) {
        this.cartao = cartao;
    }

    public DescricaoTransacao getDescricao() {
        return descricao;
    }

    public void setDescricao(DescricaoTransacao descricao) {
        this.descricao = descricao;
    }

    public FormaPagamento getFormaPagamento() {
        return formaPagamento;
    }

    public void setFormaPagamento(FormaPagamento formaPagamento) {
        this.formaPagamento = formaPagamento;
    }
}
