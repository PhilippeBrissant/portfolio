package API;

import java.util.List;

import javax.validation.Valid;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.util.UriComponentsBuilder;

import DTO.PagamentoDTO;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

import Model.TransacaoModel;
import Servico.TransacaoServico;

@RestController
@RequestMapping("/transacao")
public class TransacaoController {

    @GetMapping
    public List<TransacaoModel> consulta() {
        return new TransacaoServico().getTransacoes();
    }

    @GetMapping(value = "/{id}")
    public TransacaoModel consulta(@PathVariable long id) {
        return new TransacaoServico().getTransacaoById(id);
    }

    @PostMapping(value = "/pagamento")
    public TransacaoModel pagamento(@RequestBody @Valid PagamentoDTO pagamentoDTO, UriComponentsBuilder uriBuilder) {
        return new TransacaoServico().solicitaPagamento(pagamentoDTO);
    }

    @GetMapping(value = "/pagamento")
    public List<TransacaoModel> pagamento() {
        return new TransacaoServico().getPagamentos();
    }

    @GetMapping(value = "/pagamento/{id}")
    public TransacaoModel pagamento(@PathVariable(value = "id") long id) {
        return new TransacaoServico().getPagamentoById(id);
    }

    @GetMapping(value = "/estorno")
    public List<TransacaoModel> estorno() {
        return new TransacaoServico().getEstornos();
    }

    @GetMapping(value = "/estorno/{id}")
    public TransacaoModel estorno(@PathVariable(value = "id") long id) {
        return new TransacaoServico().getEstornoById(id);
    }
}
