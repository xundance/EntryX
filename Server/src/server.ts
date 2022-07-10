import Fastify, { FastifyInstance } from "fastify";
import middie from "@fastify/middie";

export class DoorServer {
  server: FastifyInstance;
  private port: number;

  constructor(port: number) {
    this.port = port;
    this.server = Fastify({
      logger: {
        file: "./server.log",
      },
    });
  }

  async doorServer() {
    await this.server.register(middie);
    this.server.use(require("cors")());

    this.server.listen({ port: this.port, host: "0.0.0.0" }, (err, _) => {
      if (err) {
        this.server.log.error(err);
        process.exit(1);
      } else {
        console.log(`Server listening on port ${this.port}`);
      }
    });
  }
}
