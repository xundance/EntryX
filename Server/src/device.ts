export class Device {
  ip: string;
  address: string;
  credit: number;

  constructor(ip: string, address: string, credit: number) {
    this.ip = ip;
    this.address = address;
    this.credit = credit;
  }
}
