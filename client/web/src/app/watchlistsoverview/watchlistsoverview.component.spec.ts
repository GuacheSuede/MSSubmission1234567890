import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { WatchlistsoverviewComponent } from './watchlistsoverview.component';

describe('WatchlistsoverviewComponent', () => {
  let component: WatchlistsoverviewComponent;
  let fixture: ComponentFixture<WatchlistsoverviewComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ WatchlistsoverviewComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(WatchlistsoverviewComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
