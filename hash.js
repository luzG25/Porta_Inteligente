const crypto = require("crypto");

pass = "F9 6E 4F 05";

const hash = crypto.createHash("sha256").update(pass).digest("hex");

console.log(hash);
