# Temporizador OneShot 


Esse programa demonstra algumas das possibilidades quando reunimos um Timer Repetitivo (**repeating_timer_callback**) e um Alarme OneShot, capaz de agendar tarefas para um momento futuro (**add_alarm_in_ms**), permitindo ao microcontrolador desempenhar outras atribuições enquanto o tempo agendado não for excedido.

Nesse exercício, quando o programa é iniciado, a função **add_repeating_timer_ms** é chamada à habilitar o timer e sua função callback **repeating_timer_callback**. Em seguida, a lógica de detecção do botão é definida por uma interrupção especifica para tratar do gerenciamento e ação do click do botão e suas contramedidas deboucing. 

Enquanto isso, o loop e a CPU são devidamente atendidos com a função de otimização '**tight_loop_contents**'.

A função de callback da interrupção não só é responsável pelo botão, mas pela importante tarefa de habilitar a manipulação do **delay_ms**, varável e parâmetro responsável pelo tempo de transição do loop gerado pela função callback do timer. A importância do controle dessa variável, em tempo de execução, se dá pela necessidade em manter a sincronia e fluidez na transição entre o click do botão e o primeiro acionamento dos LEDs, mais precisamente em minimizar o efeito provocado pela transição de 100 milisegundos, utilizados pela interrupção do botão e os 3000 milisegundos necessários a cada ciclo de acionamento dos LEDs. 

Essa lógica de tempo está presente juntamente com a função que habilita o alarme e, consequentente, o tempo de vida dos ciclos de acionamentos, no callback do timer. O acesso a esse bloco temporal se dá pela condicional que avalia se houve a interrupção por parte do botão e por consequência a tão necessária alteração da variável de delay.
No momento seguinte, o programa ativa os LEDs, e os ciclos se sucedem até o momento que que o tempo de vida do alarme atinge os 9000 milisegundos, tempo exato dos três ciclos de acionamentos, no instante que a função '**turn_off_callback**' do alarme a acionada. 

Antes do seu fim, o callback do alarme desliga o último dos LEDs e, mais uma vez, o timer é alterado em seu tempo, retornando em 100 milisegundos, em seu estado inicial, apto para outra interrupção do botão. 
