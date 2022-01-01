/*
 * Copyright 2021 Conner Macolley
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */


#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "watchlist.h"
#include "../../globals.h"
#include "../utils.h"
#include <time.h>


bool tcp_portscan_detect(const struct watchlist_member * w){
  // printf("%s:  %d\n",w->ip_addr, w->rst_pkt_recv);
  int subtimes[20];
  int subtime_t = 0;
  if(w->rst_pkt_recv != 20) return false;
  for(int i = w->rst_pkt_recv - 1; i !=0;  ){
    // if(w->last_rst_pkt_times[i] == w->last_rst_pkt_times[0])  break;
    int temp = i;
    
    
    subtimes[subtime_t++] = w->last_rst_pkt_times[i] - w->last_rst_pkt_times[--i];
    if(temp == 1) break;
  }
  int f = 0;
  for(int i = 0; i < subtime_t; i++){
    if(subtimes[i] < 14843508) f++;
  }
  if(f == subtime_t){
    FILE * siglog_fp = fopen(def_log_file,"a");
    printf("Portscan Detected from IP Address %s\n",w->ip_addr);
    char message[128];
    sprintf(message,"Portscan detected from %s at %s\n",w->ip_addr,get_formated_time());
    fputs(message,siglog_fp); 
    fclose(siglog_fp);
    return true;
  }
  return false;
  
}



void member_ready_for_scan(){
  for(int i = 0; i < watchlist_num + 1;i++){
    const struct watchlist_member * w = &watchlist[i];
    if(w->rst_pkt_recv >= 20){
      
      tcp_portscan_detect(w);
    }
  }
}

int member_exists(const char * ip){
  int i = 0;
  for( ; i < watchlist_num + 1;){
    if(strcmp(watchlist[i++].ip_addr, ip) == 0) return i;
  }
  return -1;
}


void init_member(const char * ip){
  int watchlist_index;
  struct watchlist_member * w;
  if((watchlist_index = member_exists(ip)) != -1){
    return;
  }
  if((watchlist_index = member_exists("0.0.0.1")) != -1) 
    w = &watchlist[watchlist_index];
  else 
    w = &watchlist[++watchlist_num];
  
  
  strcpy(w->ip_addr,ip);
  w->nmap_watch_host_alive_watch.num_done = 0;
  w->nmap_watch_host_alive_watch.start_time = 0;
  w->rst_pkt_recv = 0;
  w->nmap_watch_host_alive_watch.tcp_ack_sent = 0;
  w->nmap_watch_host_alive_watch.tcp_syn_sent = 0;
  w->nmap_watch_host_alive_watch.num_done = 0;
  w->nmap_watch_host_alive_watch.end_time = 0;
  w->nmap_watch_host_alive_watch.icmp_echo_sent = 0;
  w->nmap_watch_host_alive_watch.icmp_time_req_sent = 0;
  w->nmap_watch_host_alive_watch.start_time = 0;
  memset(&w->last_rst_pkt_times,0,sizeof(w->last_rst_pkt_times));
}
