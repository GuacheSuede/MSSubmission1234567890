import { TestBed } from '@angular/core/testing';

import { FundamentalsService } from './fundamentals.service';

describe('FundamentalsService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: FundamentalsService = TestBed.get(FundamentalsService);
    expect(service).toBeTruthy();
  });
});
