import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { WatchlistviewComponent } from './watchlistview.component';

describe('WatchlistviewComponent', () => {
  let component: WatchlistviewComponent;
  let fixture: ComponentFixture<WatchlistviewComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ WatchlistviewComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(WatchlistviewComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
