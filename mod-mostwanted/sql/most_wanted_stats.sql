CREATE TABLE IF NOT EXISTS `most_wanted_stats` (
  `guid` int(10) unsigned NOT NULL,
  `kills` int(10) unsigned NOT NULL DEFAULT 0,
  `last_kill_time` int(10) unsigned NOT NULL DEFAULT 0,
  `faction` tinyint(3) unsigned NOT NULL COMMENT '0 for Alliance, 1 for Horde',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;