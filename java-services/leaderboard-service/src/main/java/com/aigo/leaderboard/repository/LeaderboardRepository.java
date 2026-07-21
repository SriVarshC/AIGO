package com.aigo.leaderboard.repository;

import com.aigo.leaderboard.model.LeaderboardEntry;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import java.util.List;
import java.util.Optional;

@Repository
public interface LeaderboardRepository extends JpaRepository<LeaderboardEntry, Long> {
    Optional<LeaderboardEntry> findByPlayerId(Long playerId);
    List<LeaderboardEntry> findByRegionOrderByEloRatingDesc(String region);
    List<LeaderboardEntry> findAllByOrderByEloRatingDesc();
}