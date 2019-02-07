import { Injectable } from '@angular/core';
import { watchlist } from './watchlist';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders } from '@angular/common/http';


const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable({
  providedIn: 'root'
})
export class IexService {
  iex_stock_price_url = "https://api.iextrading.com/1.0/stock/TICKER/price";
  
	constructor(private http: HttpClient) { }

	get_price(ticker: string): Observable<string> {
		// return of(watchlists);
		return this.http.get<string>(this.iex_stock_price_url.replace("TICKER", ticker));
	}

	// new_watchlist(watchlist_item: watchlist): Observable<watchlist>{
	// 	return this.http.post<watchlist>(this.watchlist_post_url, watchlist_item, httpOptions);
	// 	//security not needed.
	// }



}
