# Telecomando NEC
Realizzare un telecomando che trasmetta codici NEC in base al tasto digitato su un tastierino 4x3. Il keypad deve essere gestito in background e non usare funzioni di libreria ed evitando rimbalzi. Le funzioni di trasmissione su led dovranno essere sincronizzate a un clock ed essere quindi indipendenti e temporizzate tramite Interrupt del clock. “loop()” dovrà essere quindi nulla.

# Simulazione
La simulazione della versione finale del Telecomando si trova a questo link:
[Telecomando - Completo](https://wokwi.com/projects/374504307076985857)

# File del Logic Analyzer
Nella cartella "File LogicAnalyzer" sono presenti tutti i file .vcd su cui ho fatto i vari test descritti nella Relazione. E' inoltre presente un ulteriore file "Nec_TuttiTasti.vcd" dove sono presenti le trasmissioni NEC di tutti i tasti del Keypad.
