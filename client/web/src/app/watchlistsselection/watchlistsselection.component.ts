import { Component, OnInit } from '@angular/core';
import { watchlists } from '../mock-watchlist';
import { WatchlistService } from '../watchlist.service'
import { watchlist } from '../watchlist'
import {interval} from "rxjs/internal/observable/interval";
import { map, filter, switchMap } from 'rxjs/operators';

@Component({
  selector: 'app-watchlistsselection',
  templateUrl: './watchlistsselection.component.html',
  styleUrls: ['./watchlistsselection.component.sass']
})
export class WatchlistsselectionComponent implements OnInit {
  // watchlistsdata = watchlists;
  watchlistsdata: watchlist[];
  selectedwatchlist: string[];

  constructor(private watchlistservice: WatchlistService) { }

  ngOnInit() {
    this.get_watchlists()
  }

  get_watchlists(): void {
    interval(1000).pipe(switchMap(() =>  this.watchlistservice.get_watchlists())).subscribe( watchlists => this.watchlistsdata = watchlists );
  }

  select_watchlist(name: string): void{
    this.selectedwatchlist = this.watchlistsdata.filter( wl => {
      return wl.name == name;
    })[0].stocks;
    console.log(this.selectedwatchlist)
  }

}

//just subling for now, we are shipping, fuck learning new
