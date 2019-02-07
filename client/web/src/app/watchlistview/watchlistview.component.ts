import { Component, OnInit, Input } from '@angular/core';
import { watchlist } from '../watchlist';

@Component({
  selector: 'app-watchlistview',
  templateUrl: './watchlistview.component.html',
  styleUrls: ['./watchlistview.component.sass']
})
export class WatchlistviewComponent implements OnInit {
  @Input("watchlist") watchlistitems: string[];
  

  constructor() { }

  ngOnInit() {
  }
}
