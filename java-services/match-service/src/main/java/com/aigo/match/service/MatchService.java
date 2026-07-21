package com.aigo.match.service;

import com.aigo.match.model.Match;
import com.aigo.match.repository.MatchRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

@Service
public class MatchService {

    @Autowired
    private MatchRepository matchRepository;

    public List<Match> getAllMatches() {
        return matchRepository.findAllByOrderByPlayedAtDesc();
    }

    public Optional<Match> getMatchById(Long id) {
        return matchRepository.findById(id);
    }

    public Optional<Match> getMatchByMatchId(String matchId) {
        return matchRepository.findByMatchId(matchId);
    }

    public Match createMatch(Match match) {
        if (match.getPlayedAt() == null) {
            match.setPlayedAt(LocalDateTime.now());
        }
        return matchRepository.save(match);
    }

    public List<Match> getMatchesByPlayer(Long playerId) {
        return matchRepository.findByPlayer1IdOrPlayer2Id(playerId, playerId);
    }

    public List<Match> getMatchesByArena(String arenaName) {
        return matchRepository.findByArenaName(arenaName);
    }
}
