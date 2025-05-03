'use client';

import { Search } from 'lucide-react';
import { useState } from 'react';
import { useRouter } from 'next/navigation';

export default function Home() {
  const [searchQuery, setSearchQuery] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const router = useRouter();

  const handleSearch = async () => {
    if (!searchQuery.trim()) return;
    setIsLoading(true);
    router.push(`/results?q=${encodeURIComponent(searchQuery)}`);
  };

  return (
    <div className="min-h-screen bg-[#0F0A1F] text-white flex flex-col items-center justify-center p-8">
      <div className="max-w-3xl w-full text-center space-y-8">
        {/* Logo/Title */}
        <h1 className="text-7xl font-bold mb-4">
          Nexa
        </h1>
        
        {/* Subtitle */}
        <p className="text-xl text-gray-400 mb-12">
          Search companies through unstructured natural language
        </p>

        {/* Search Bar */}
        <div className="relative w-full">
          <input
            type="text"
            value={searchQuery}
            onChange={(e) => setSearchQuery(e.target.value)}
            onKeyDown={(e) => e.key === 'Enter' && handleSearch()}
            placeholder="Food delivery company in San Francisco"
            className="w-full px-6 py-4 bg-[#1A1425] rounded-full text-lg focus:outline-none focus:ring-2 focus:ring-purple-500 pl-6 pr-12"
          />
          <button 
            onClick={handleSearch}
            disabled={isLoading}
            className="absolute right-4 top-1/2 transform -translate-y-1/2 disabled:opacity-50"
          >
            <Search className="w-6 h-6 text-gray-400" />
          </button>
        </div>

        {/* Loading State */}
        {isLoading && (
          <div className="text-purple-400">Redirecting...</div>
        )}
      </div>
    </div>
  );
}
