import { X } from "./x";
import { DoorServer } from "./server";
import { Device } from "./device";

const entryCost = 1000000; //in drops
const ds = new DoorServer(8070);
var mainDoor = new Device(
  "192.168.1.1..",
  "r...",
  0
);
var sideDoor = new Device(
  "192.168.1.1..",
  "r...",
  0
);
const accs = [sideDoor, mainDoor];
const x = new X(
  "wss://xrplcluster.com",
  accs.map((a) => a.address)
);

process.stdin.resume();
async function exitHandler(options: any, exitCode: any) {
  if (options.cleanup) console.log("clean");
  await x.unsubscribeAccounts();
  await x.disconnect();
  if (exitCode || exitCode === 0) console.log(exitCode);
  if (options.exit) process.exit();
}
process.on("exit", exitHandler.bind(null, { cleanup: true }));
process.on("SIGINT", exitHandler.bind(null, { exit: true }));
process.on("uncaughtException", exitHandler.bind(null, { exit: true }));

try {
  doors();
} catch (error) {
  throw error;
}

async function doors() {
  await x.connect();
  await x.subscribeAccounts();
  await ds.doorServer();
  ds.server.get("/knock", async (req, rep) => {
    console.log(req.ip, "knocked.");
    var idx = accs.findIndex((i) => i.ip === req.ip);
    var open = false;
    if (idx !== -1) open = getCredit(idx);
    if (open) {
      // TODO: Logging
      console.log(req.ip, ": Open!");
      rep.status(200);
      return "Open";
    } else {
      // TODO: Logging
      console.log(req.ip, ": Closed!");
      rep.status(500);
      return "Closed";
    }
  });
  x.api.on("transaction", (tx) => {
    // console.log(tx);
    if (tx.transaction.TransactionType == "Payment") {
      console.log("Incoming payment...");
      var idx = accs.map((i) => i.address).indexOf(tx.transaction.Destination);
      if (idx !== -1) setCredit(+tx.transaction.Amount, idx);
    }
  });
}

function setCredit(amount: number, idx: number) {
  if (amount >= entryCost) {
    console.log("Payment OK. Updated status on door", accs[idx]);
    accs[idx]!.credit += Math.floor(amount / entryCost);
  }
}

function getCredit(idx: number) {
  if (accs[idx]!.credit >= 1) {
    accs[idx]!.credit--;
    return true;
  } else return false;
}
