import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders } from '@angular/common/http';


const httpOptions = {
  headers: new HttpHeaders({ 
    'Content-Type': 'application/json'  ,
    'Authorization': 'Basic ' + btoa("admin:changeit")
  }
)};

@Injectable({
  providedIn: 'root'
})
export class FundamentalsService {

	fundamentals_get_url = "http://127.0.0.1:8081/db/fundamentals?pagesize=1000&filter={\'tic\': \'TICKER\'}&sort_by=datafqtr"		

	constructor(private http: HttpClient) { }

	get_fundamentals(ticker: string): Observable<string> {
    return this.http.get<string>(this.fundamentals_get_url.replace("TICKER", ticker), httpOptions);

	}




}
