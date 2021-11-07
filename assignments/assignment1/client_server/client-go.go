/*****************************************************************************
 * client-go.go                                                                 
 * Name:
 * NetId:
 *****************************************************************************/

package main

import (
  "os"
  "log"
  "net"
)

const SEND_BUFFER_SIZE = 2048

/* TODO: client()
 * Open socket and send message from stdin.
*/
func client(server_ip string, server_port string) {
  buffer := make([]byte, SEND_BUFFER_SIZE)
  // set up TCP connect
  conn, err := net.Dial("tcp", server_ip+":"+server_port)
  if err != nil {
    log.Fatalf("%s Unable to connect %s:%s", err, server_ip, server_port)
  }
  for data_size, err := os.Stdin.Read(buffer); err == nil; data_size, err = os.Stdin.Read(buffer){
    if _, err = conn.Write(buffer[:data_size]); err != nil {
      log.Fatal(err)
    }
  }
}

// Main parses command-line arguments and calls client function
func main() {
  if len(os.Args) != 3 {
    log.Fatal("Usage: ./client-go [server IP] [server port] < [message file]")
  }
  server_ip := os.Args[1]
  server_port := os.Args[2]
  client(server_ip, server_port)
}
