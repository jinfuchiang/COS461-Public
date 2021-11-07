/*****************************************************************************
 * server-go.go                                                                 
 * Name:
 * NetId:
 *****************************************************************************/

package main

import (
  "io"
  "os"
  "log"
  "net"
)

const RECV_BUFFER_SIZE = 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
*/
func server(server_port string) {
  ln, err := net.Listen("tcp", ":"+server_port)
  if err != nil {
    log.Fatalf("%s: Unable to listen", err)
  }
  for {
    conn, err := ln.Accept()
    if err != nil {
      log.Fatal(err)
    }
    // status, err := bufio.NewReader(conn).ReadString
    io.Copy(os.Stdout, conn)
  }
}


// Main parses command-line arguments and calls server function
func main() {
  if len(os.Args) != 2 {
    log.Fatal("Usage: ./server-go [server port]")
  }
  server_port := os.Args[1]
  server(server_port)
}
