// rfid-serial-server.js
// Node.js: lê UID do Arduino via serial e valida

const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");
const crypto = require("crypto");

// === CONFIGURAÇÕES ===
// Ajusta o caminho da porta serial ao teu sistema (ex: '/dev/ttyUSB0' ou 'COM3')
const SERIAL_PATH = "/dev/ttyUSB0";
const BAUD_RATE = 9600;

// Lista de UIDs autorizados (mesmo formato que o Arduino envia: ex "F9 6E 4F 05")
const AUTHORIZED_UIDS = new Set([
  "5afe7cb82c596aa3f4bf64132dffd44cb6043d4c9d51e4df0f5f23e1ef9963a9", //F9 6E 4F 05
  "52aa8c49fa33b0ec8935a1fac93b2f6bf0fbe3ce23d342578027379d6b06d829", //BE 89 B7 01
]);

// Se true, depois de validar com sucesso envia "OPEN" ao Arduino
const SEND_OPEN_COMMAND = true;

// === FIM CONFIG ===

const port = new SerialPort({ path: SERIAL_PATH, baudRate: BAUD_RATE });
const parser = port.pipe(new ReadlineParser({ delimiter: "\n" }));

port.on("open", () => {
  console.log("Porta serial aberta em", SERIAL_PATH, "baud", BAUD_RATE);
});

port.on("error", (err) => {
  console.error("Erro na porta serial:", err.message);
});

// Parser emite cada linha que o Arduino envia
parser.on("data", (line) => {
  const texto0 = line.toString().trim();
  console.log(texto0);
  const texto1 = texto0.split(":");

  if (texto1[0] === "ACK") {
    //porta foi fechada
  }
  if (texto1[0] === "UID") {
    const texto = texto1[1].trim();

    // Exibe tudo (útil para debug)
    console.log("[serial]", texto);

    if (verifyAccess(texto)) {
      console.log("Acesso concedido");
      port.write("OPEN\n");
    } else {
      console.log("ACESSO NEGADO");
      port.write("DENIED\n");
    }
  }

  /*
  // Tentamos extrair o UID de linhas que contenham "Codigo do RFID:" ou "UID lido:" (caso o Arduino use outro texto)
  // Exemplos suportados:
  // Codigo do RFID: BD 9B 06 7D
  // UID lido: F9 6E 4F 05
  const regex =
    /(?:Codigo do RFID:|UID lido:|UID da tag|Codigo do RFID)\s*:?\s*([0-9A-Fa-f ]{7,})/;
  const m = texto.match(regex);
  if (!m) {
    // Não é uma linha de UID — ignora
    return;
  }

  // Normaliza: maiúsculas, remove espaços extras
  const uid = m[1].trim().toUpperCase().replace(/\s+/g, " ");
  console.log("UID detectado ->", uid);

  if (AUTHORIZED_UIDS.has(uid)) {
    console.log(">> Acesso autorizado para", uid);
    if (SEND_OPEN_COMMAND) {
      // envia comando simples; Arduino deve escutar por "OPEN\n"
      port.write("OPEN\n", (err) => {
        if (err) console.error("Erro ao enviar OPEN:", err.message);
        else console.log("Comando OPEN enviado ao Arduino.");
      });
    }
  } else {
    console.log(">> Acesso NEGADO para", uid);
  } */
});

const hashSHA256 = (texto) => {
  return crypto.createHash("sha256").update(texto).digest("hex");
};

const verifyAccess = (pass) => {
  return AUTHORIZED_UIDS.has(
    crypto.createHash("sha256").update(pass).digest("hex")
  );
};
