package API;

import java.util.List;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;

import DAO.TransacaoDAO;
import DTO.PagamentoDTO;
import Model.TransacaoModel;

@SpringBootTest
public class TransacaoTests {

    private TransacaoDAO dao = new TransacaoDAO();

    @Test
    public void deveRecuperarTodasTransacoes(){
        List<TransacaoModel> transacoes = dao.getAllTransacoes();
        Assertions.assertNotNull(transacoes);
    }

    @Test
    public void deveRecuperarUmaTransacaoPeloID(){
        TransacaoModel pagamento = dao.getTransacaoById(2);
        Assertions.assertNotNull(pagamento);
    }

    @Test
    public void deveRecuperarTodosPagamentos(){
        List<TransacaoModel> pagamentos = dao.getAllPagamentos();
        Assertions.assertNotNull(pagamentos);
    }

    @Test
    void deveSalvarPagamento() {
        String cartao = "123";
        String descricao = "desc";
        String valor = "22";
        int formaPagamento = 3;
        String parcelas = "10";

        PagamentoDTO pagamento = new PagamentoDTO(cartao, descricao, valor, formaPagamento, parcelas);
        int retorno = new TransacaoDAO().inserePagamento(pagamento.getPagamento());

        Assertions.assertTrue(retorno > 0);
    }

    @Test
    public void deveRecuperarUmPagamentoPeloID(){
        TransacaoModel pagamento = dao.getPagamentoById(10);
        Assertions.assertNotNull(pagamento);
    }
}
