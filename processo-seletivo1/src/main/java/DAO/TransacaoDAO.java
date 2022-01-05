package DAO;

import Model.TransacaoModel;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Random;
import java.util.stream.Collectors;

import Model.DescricaoTransacao;
import Model.FormaPagamento;
import enums.StatusTransacao;

public class TransacaoDAO {
    private static ArrayList<TransacaoModel> transacoes = new ArrayList<TransacaoModel>();

    public TransacaoDAO() {
        if (transacoes.isEmpty())
            insereEstorno();
    }

    private void insereEstorno() {
        for (int i = 0; i < 10; i++) {
            TransacaoModel model = new TransacaoModel();
            model.setId(i);
            model.setCartao("4444********1234");
            model.setDescricao(new DescricaoTransacao(500.50, new Date(), "PetShop MundoCão")
                    .setNsu("1234567890")
                    .setCodigoAutorizacao("147258369")
                    .setStatus(StatusTransacao.CANCELADO));

            if (i < 3)
                model.setFormaPagamento(new FormaPagamento(FormaPagamento.Tipo.AVISTA, 0));
            else if (i < 6)
                model.setFormaPagamento(
                        new FormaPagamento(FormaPagamento.Tipo.PARCELADO_EMISSOR, new Random().nextInt(24)));
            else if (i < 10)
                model.setFormaPagamento(
                        new FormaPagamento(FormaPagamento.Tipo.PARCELADO_LOJA, new Random().nextInt(24)));

            transacoes.add(model);
        }

    }

    private ArrayList<TransacaoModel> getTransacoes() {
        return transacoes;
    }

    public List<TransacaoModel> getAllTransacoes() {
        return getTransacoes();
    }

    public TransacaoModel getTransacaoById(long id) throws NoSuchElementException {
        return getTransacoes().stream().filter(t -> t.getId() == id).findFirst().get();
    }

    public TransacaoModel getTransacaoByIdEStatus(long id, StatusTransacao status) throws NoSuchElementException {
        return getTransacoes().stream().filter(t -> t.getId() == id && t.getDescricao().getStatus() == status)
                .findFirst().get();
    }

    public List<TransacaoModel> getAllPagamentos() throws NoSuchElementException {
        return getTransacoes().stream().filter(t -> t.getDescricao().getStatus() != StatusTransacao.CANCELADO)
                .collect(Collectors.toList());
    }

    public TransacaoModel getPagamentoById(long id) throws NoSuchElementException {
        return getTransacoes().stream()
                .filter(t -> t.getId() == id && t.getDescricao().getStatus() != StatusTransacao.CANCELADO)
                .findFirst().get();
    }

    public int inserePagamento(TransacaoModel pagamento) {
        pagamento.getDescricao()
                .setNsu("123123123")
                .setCodigoAutorizacao("123123");

        float aux = new Random().nextFloat() * 3;
        if (aux > 2.3)
            pagamento.getDescricao().setStatus(StatusTransacao.NEGADO);
        else
            pagamento.getDescricao().setStatus(StatusTransacao.AUTORIZADO);

        transacoes.add(pagamento);
        return transacoes.size() - 1;
    }

    public List<TransacaoModel> getAllEstorno() throws NoSuchElementException {
        return getTransacoes().stream().filter(t -> t.getDescricao().getStatus() == StatusTransacao.CANCELADO)
                .collect(Collectors.toList());

    }
}
