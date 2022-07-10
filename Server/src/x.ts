import * as xrpl from "xrpl";

export class X {
  private accs: string[];
  api: xrpl.Client;

  constructor(url: string, accs: string[]) {
    this.api = new xrpl.Client(url);
    this.accs = accs;
  }

  async connect() {
    try {
      await this.api.connect();
    } catch (error) {
      throw error;
    }
  }

  async disconnect() {
    try {
      await this.api.disconnect();
    } catch (error) {
      throw error;
    }
  }

  async subscribeAccounts() {
    try {
      const response = await this.api.request({
        id: "doors",
        command: "subscribe",
        accounts: this.accs,
      });
      return true;
    } catch (error) {
      throw error;
    }
  }

  async unsubscribeAccounts() {
    try {
      const response = await this.api.request({
        id: "doors",
        command: "unsubscribe",
        accounts: this.accs,
      });
      return true;
    } catch (error) {
      throw error;
    }
  }
}
