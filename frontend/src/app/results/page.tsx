'use client';

import { useEffect, useState } from 'react';
import { useSearchParams } from 'next/navigation';
import dynamic from 'next/dynamic';
import { ChevronLeft, ChevronRight } from 'lucide-react';

interface Company {
  id: number,
  name: string;
  specificIndustry: string;
  location: string;
}

interface GraphData {
  nodes: Array<{ id: number; name: string }>;
  links: Array<{ source: number; target: number; value: number }>;
}

// Dynamically import the Graph component with no SSR
const Graph3D = dynamic(() => import('./components/Graph3D'), {
  ssr: false,
  loading: () => (
    <div className="w-full h-full flex items-center justify-center text-gray-400">
      Loading 3D visualization...
    </div>
  ),
});

export default function Results() {
  const searchParams = useSearchParams();
  const [allResults, setAllResults] = useState<Company[]>([]);
  const [page, setPage] = useState(1);
  const resultsPerPage = 5;
  const [graphData, setGraphData] = useState<GraphData | null>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const query = searchParams.get('q');
    if (!query) {
      setError('No search query provided');
      setIsLoading(false);
      return;
    }

    const fetchResults = async () => {
      try {
        const query = searchParams.get('q');
        const response = await fetch('/api/search', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({ query }),
        });
    
        if (!response.ok) {
          throw new Error('Search request failed');
        }
    
        const data = await response.json();
        if (data.error) {
          throw new Error(data.error);
        }
    
        setAllResults(data);
    
        const firstCompanyId = data[0]?.id?.toString();
        if (!firstCompanyId) {
          throw new Error('First result is missing an ID');
        }
    
        const graphPostResponse = await fetch('http://localhost:3001/graph', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({ id: firstCompanyId }),
        });
    
        if (!graphPostResponse.ok) {
          const errorText = await graphPostResponse.text();
          throw new Error(`Graph API error: ${graphPostResponse.status} ${errorText}`);
        }        
    
        const graphData = await graphPostResponse.json();
          setGraphData(graphData);
          setError(null);
        } catch (err) {
          console.error('Error:', err);
          setError(err instanceof Error ? err.message : 'An unexpected error occurred');
          setAllResults([]);
          setGraphData(null);
        } finally {
          setIsLoading(false);
        }
    };
    
    fetchResults();
  }, [searchParams]);

  // Pagination logic
  const totalPages = Math.ceil(allResults.length / resultsPerPage);
  const startIdx = (page - 1) * resultsPerPage;
  const endIdx = startIdx + resultsPerPage;
  const displayedResults = allResults.slice(startIdx, endIdx);

  const goToPage = (p: number) => {
    if (p >= 1 && p <= totalPages) setPage(p);
  };

  if (isLoading) {
    return (
      <div className="min-h-screen bg-[#0F0A1F] text-white flex items-center justify-center">
        <div className="text-xl">Loading...</div>
      </div>
    );
  }

  if (error) {
    return (
      <div className="min-h-screen bg-[#0F0A1F] text-white flex items-center justify-center">
        <div className="text-xl text-red-400">{error}</div>
      </div>
    );
  }

  return (
    <div className="min-h-screen w-full flex bg-[#0F0A1F] text-white overflow-hidden">
      {/* Left: Floating results box */}
      <div className="flex flex-col justify-center items-start w-full max-w-lg pl-12">
        <div className="bg-[#1A1425]/80 backdrop-blur-md rounded-2xl shadow-2xl p-8 border border-[#2D2340] w-full">
          <h2 className="text-3xl font-bold mb-8 text-white">Most Similar Companies</h2>
          <div className="space-y-4 h-[70vh] max-h-[700px] overflow-y-auto pr-2">
            {displayedResults.map((company, index) => (
              <div
                key={index}
                className="bg-[#1A1425] p-6 rounded-xl hover:bg-[#251D34] transition-colors shadow"
              >
                <h3 className="text-xl font-semibold mb-2">{company.name}</h3>
                <p className="text-gray-400">
                  {company.location}
                </p>
                <p className="text-purple-400 mt-1">
                  {company.specificIndustry}
                </p>
              </div>
            ))}
          </div>
          {/* Pagination Controls */}
          {totalPages > 1 && (
            <div className="flex items-center justify-center gap-2 pt-8 select-none">
              {/* Left Chevron */}
              <button
                onClick={() => goToPage(page - 1)}
                disabled={page === 1}
                className={`rounded-full p-2 ${page === 1 ? 'text-gray-500 cursor-not-allowed' : 'hover:bg-[#251D34] text-white'}`}
                aria-label="Previous page"
              >
                <ChevronLeft size={24} />
              </button>
              {/* Page Numbers */}
              {Array.from({ length: totalPages }, (_, i) => (
                <button
                  key={i + 1}
                  onClick={() => goToPage(i + 1)}
                  className={`w-8 h-8 flex items-center justify-center rounded-full mx-1 text-lg font-semibold ${page === i + 1 ? 'bg-purple-600 text-white' : 'text-gray-300 hover:bg-[#251D34]'}`}
                  aria-current={page === i + 1 ? 'page' : undefined}
                >
                  {i + 1}
                </button>
              ))}
              {/* Right Chevron */}
              <button
                onClick={() => goToPage(page + 1)}
                disabled={page === totalPages}
                className={`rounded-full p-2 ${page === totalPages ? 'text-gray-500 cursor-not-allowed' : 'hover:bg-[#251D34] text-white'}`}
                aria-label="Next page"
              >
                <ChevronRight size={24} />
              </button>
            </div>
          )}
        </div>
      </div>
      {/* Right: 3D Graph centered */}
      <div className="flex-1 flex items-center justify-center h-screen relative min-h-screen bg-transparent">
        <div className="absolute inset-0">
          {graphData ? <Graph3D data={graphData} /> : <div className='flex items-center justify-center h-full text-gray-400'>Loading graph...</div>}
        </div>
      </div>
    </div>
  );
} 