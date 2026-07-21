package com.aigo.match.service;

import com.aigo.match.model.Match;
import com.aigo.match.repository.MatchRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

@Service
public class MatchService {

    @Autowired
    private MatchRepository matchRepository;

    @Cacheable("matches")
    public List<Match> getAllMatches() {
        return matchRepository.findAllByOrderByPlayedAtDesc();
    }

    @Cacheable(value = "matches", key = "#id")
    public Optional<Match> getMatchById(Long id) {
        return matchRepository.findById(id);
    }

    @Cacheable(value = "matchesByCode", key = "#matchId")
    public Optional<Match> getMatchByMatchId(String matchId) {
        return matchRepository.findByMatchId(matchId);
    }

    @CacheEvict(value = {"matches", "matchesByCode", "matchesByPlayer", "matchesByArena"}, allEntries = true)
    public Match createMatch(Match match) {
        if (match.getPlayedAt() == null) {
            match.setPlayedAt(LocalDateTime.now());
        }
        return matchRepository.save(match);
    }

    @Cacheable(value = "matchesByPlayer", key = "#playerId")
    public List<Match> getMatchesByPlayer(Long playerId) {
        return matchRepository.findByPlayer1IdOrPlayer2Id(playerId, playerId);
    }

    @Cacheable(value = "matchesByArena", key = "#arenaName")
    public List<Match> getMatchesByArena(String arenaName) {
        return matchRepository.findByArenaName(arenaName);
    }
}
