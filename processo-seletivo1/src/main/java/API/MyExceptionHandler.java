package API;

import java.util.NoSuchElementException;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.servlet.mvc.method.annotation.ResponseEntityExceptionHandler;

@ControllerAdvice
public class MyExceptionHandler extends ResponseEntityExceptionHandler {
    
    @ExceptionHandler(NoSuchElementException.class)
    public ResponseEntity<Object> pegaNoSuchElementException(){
        return ResponseEntity.status(HttpStatus.NOT_FOUND).build();
    }
    
    @ExceptionHandler(NumberFormatException.class)
    public ResponseEntity<Object> pegaNumberFormatException(){
        return ResponseEntity.status(HttpStatus.BAD_REQUEST).build();
    }
}
