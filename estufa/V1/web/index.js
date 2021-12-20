$(document).ready(function () {
    GetArduinoInfo();
});

function GetArduinoInfo() {
    console.log("fazendo a requisição");
    //nocache = "&nocache=" + Math.random() * 1000000;
    $.get("http://192.168.0.177/ajax_inputs", function (data) {
        //sempre virá nessa ordem: luz, subtexto, solo, bomba, temp, ventilador
        dados = data.split("|");
        console.log(dados)

        //seta luz
        if (dados[0] == 1)
            $('#lblLuz').html('Luz acesa');
        else
            $('#lblLuz').html('Luz apagada');

        //Seta sub texto Luz
        $('#lblLuzSub').html('Tempo restante: ' + dados[1]);

        //Seta solo
        if (dados[2] == 1)
            $('#lblSolo').html('Solo úmido');

        else
            $('#lblSolo').html('Solo seco');


        //Seta bomba
        if (dados[3] == 1)
            $('#lblSoloSub').html('Bomba ligada');

        else
            $('#lblSoloSub').html('Bomba desligada');


        //Seta temperatura
        $('#lblTemperatura').html('Temp ' + dados[4] + 'º');

        //Seta ventilador
        if (dados[5] == 1)
            $('#lblTemperaturaSub').html('Ventilador ligado');
        else
            $('#lblTemperaturaSub').html('Ventilador desligado');
    });

    setTimeout('GetArduinoInfo()', 1000);
}