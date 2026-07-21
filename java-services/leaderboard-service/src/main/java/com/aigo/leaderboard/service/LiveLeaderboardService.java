package com.aigo.leaderboard.service;

import com.aigo.leaderboard.model.LeaderboardEntry;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.redis.core.ZSetOperations;
import org.springframework.stereotype.Service;

import java.util.*;

// Real-time leaderboard backed by a Redis sorted set (ZSET), maintained
// alongside Postgres (the source of truth). ZSET gives O(log N) rank/score
// lookups instead of an ORDER BY query over the whole table - the standard
// approach for live game leaderboards.
@Service
public class LiveLeaderboardService {

    private static final String ZSET_KEY = "leaderboard:live:elo";
    private static final String META_HASH_KEY = "leaderboard:live:meta";

    @Autowired
    private StringRedisTemplate redisTemplate;

    public void syncEntry(LeaderboardEntry entry) {
        String member = entry.getPlayerId().toString();
        redisTemplate.opsForZSet().add(ZSET_KEY, member, entry.getEloRating());
        String meta = entry.getPlayerName() + "|" + entry.getRegion() + "|" + entry.getRank();
        redisTemplate.opsForHash().put(META_HASH_KEY, member, meta);
    }

    public List<Map<String, Object>> getTopN(int n) {
        Set<ZSetOperations.TypedTuple<String>> tuples =
            redisTemplate.opsForZSet().reverseRangeWithScores(ZSET_KEY, 0, n - 1);
        List<Map<String, Object>> result = new ArrayList<>();
        if (tuples == null) return result;
        int position = 1;
        for (ZSetOperations.TypedTuple<String> tuple : tuples) {
            result.add(buildRow(tuple.getValue(), tuple.getScore(), position++));
        }
        return result;
    }

    public Optional<Map<String, Object>> getPlayerRank(Long playerId) {
        String member = playerId.toString();
        Long rank = redisTemplate.opsForZSet().reverseRank(ZSET_KEY, member);
        Double score = redisTemplate.opsForZSet().score(ZSET_KEY, member);
        if (rank == null || score == null) {
            return Optional.empty();
        }
        return Optional.of(buildRow(member, score, rank.intValue() + 1));
    }

    private Map<String, Object> buildRow(String member, Double score, int position) {
        Object metaObj = redisTemplate.opsForHash().get(META_HASH_KEY, member);
        String meta = metaObj != null ? metaObj.toString() : "Unknown||";
        String[] parts = meta.split("\\|", -1);

        Map<String, Object> row = new LinkedHashMap<>();
        row.put("position", position);
        row.put("playerId", Long.valueOf(member));
        row.put("playerName", parts.length > 0 ? parts[0] : "Unknown");
        row.put("region", parts.length > 1 ? parts[1] : "");
        row.put("rank", parts.length > 2 ? parts[2] : "");
        row.put("eloRating", score.intValue());
        return row;
    }
}
