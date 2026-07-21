package com.aigo.match.repository;

import com.aigo.match.model.Match;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import java.util.List;
import java.util.Optional;

@Repository
public interface MatchRepository extends JpaRepository<Match, Long> {
    Optional<Match> findByMatchId(String matchId);
    List<Match> findByPlayer1IdOrPlayer2Id(Long player1Id, Long player2Id);
    List<Match> findByArenaName(String arenaName);
    List<Match> findAllByOrderByPlayedAtDesc();
}