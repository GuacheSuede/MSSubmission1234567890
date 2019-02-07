import { TestBed } from '@angular/core/testing';

import { IexService } from './iex.service';

describe('IexService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: IexService = TestBed.get(IexService);
    expect(service).toBeTruthy();
  });
});
