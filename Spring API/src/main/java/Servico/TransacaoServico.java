package Servico;

import java.util.List;
import java.util.NoSuchElementException;

import DAO.TransacaoDAO;
import DTO.PagamentoDTO;
import Model.TransacaoModel;
import enums.StatusTransacao;

public class TransacaoServico {

    public List<TransacaoModel> getTransacoes() {
        TransacaoDAO dao = new TransacaoDAO();
        return dao.getAllTransacoes();
    }

    public TransacaoModel getTransacaoById(long id) throws NoSuchElementException {
        TransacaoDAO dao = new TransacaoDAO();
        return dao.getTransacaoById(id);
    }

    public List<TransacaoModel> getPagamentos() throws NoSuchElementException {
        TransacaoDAO dao = new TransacaoDAO();
        return dao.getAllPagamentos();
    }

    public TransacaoModel getPagamentoById(long id) throws NoSuchElementException {
        TransacaoDAO dao = new TransacaoDAO();
        return dao.getTransacaoByIdEStatus(id, StatusTransacao.AUTORIZADO);
    }

    public TransacaoModel solicitaPagamento(PagamentoDTO pagamentoDTO) throws NumberFormatException {
        TransacaoModel pagamento = pagamentoDTO.getPagamento();
        TransacaoDAO dao = new TransacaoDAO();
        int id = dao.inserePagamento(pagamento);
        pagamento.setId(id);
        return pagamento;
    }

    public List<TransacaoModel> getEstornos() throws NoSuchElementException {
        TransacaoDAO dao = new TransacaoDAO();
        return dao.getAllEstorno();
    }

    public TransacaoModel getEstornoById(long id) throws NoSuchElementException {
        TransacaoDAO dao = new TransacaoDAO();
        return dao.getTransacaoByIdEStatus(id, StatusTransacao.CANCELADO);
    }

}
