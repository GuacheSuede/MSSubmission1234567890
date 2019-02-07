import { Injectable } from '@angular/core';
import { watchlist } from './watchlist';
import { watchlists } from './mock-watchlist';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders } from '@angular/common/http';


const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable({
  providedIn: 'root'
})
export class WatchlistService {
	watchlist_get_url = "http://localhost:8090/"		
	watchlist_post_url = "http://localhost:8091/"		

	constructor(private http: HttpClient) { }

	get_watchlists(): Observable<watchlist[]> {
		// return of(watchlists);
		return this.http.get<watchlist[]>(this.watchlist_get_url);
	}

	new_watchlist(watchlist_item: watchlist): Observable<watchlist>{
		return this.http.post<watchlist>(this.watchlist_post_url, watchlist_item, httpOptions);
		//security not needed.
	}



}
